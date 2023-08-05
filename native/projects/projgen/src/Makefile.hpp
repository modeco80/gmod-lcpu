#pragma once

#include <format>
#include <lucore/Assert.hpp>
#include <string>
#include <unordered_map>

#include "BaseConfig.hpp"
#include "FsUtils.hpp"
#include "Project.hpp"

namespace projgen::make {

	// base class for makefile rules
	struct MakefileGeneratable {
		virtual ~MakefileGeneratable() = default;
		virtual std::string Generate() = 0;
	};

	struct MakefileGlobalVariables : public MakefileGeneratable {
		std::unordered_map<std::string, std::string> values;

		// initalize for the global collection
		MakefileGlobalVariables(const std::string& projectName, const std::string& objects) {
			values["NAME"] = projectName;
			values["CC"] = PROJGEN_CC;
			values["CXX"] = PROJGEN_CXX;
			values["LD"] = PROJGEN_LD;
			values["OBJCOPY"] = PROJGEN_OBJCOPY;

			values["BASE_CCFLAGS"] = PROJGEN_BASE_C_FLAGS;
			values["BASE_CXXFLAGS"] = PROJGEN_BASE_CC_FLAGS;
			values["BASE_LDFLAGS"] = PROJGEN_BASE_LD_FLAGS;

			values["OBJS"] = objects;
		}

		std::string Generate() override {
			auto str = std::string();
			for(auto& kv : values) {
				str += std::format("{} = {}\n", kv.first, kv.second);
			}

			return str;
		}
	};

	struct MakefileConfiguration : public MakefileGeneratable {
		MakefileConfiguration(const std::unordered_map<std::string, projgen::Project::Configuration>& configs) : configs(configs) {}

		std::string Generate() override {
			auto string = std::string();

			for(auto& config : configs) {
				string += std::format("ifeq ($(CONFIG),{})\n", config.first);
				string += std::format("{}_Valid = yes\n", config.first);
				string += std::format("{}_CCFLAGS = $(BASE_CCFLAGS) {}\n", config.first, config.second.cCompileFlags);
				string += std::format("{}_CXXFLAGS = $(BASE_CXXFLAGS) {}\n", config.first, config.second.cppCompileFlags);
				string += std::format("{}_LDFLAGS = $(BASE_LDFLAGS) {}\n", config.first, config.second.linkerFlags.value_or(""));
				string += std::format("endif\n");
			}

			string += std::format("ifeq ($(CONFIG),)\n$(error Please specify a build configuration)\nendif\n");
			string += std::format("ifneq ($($(CONFIG)_Valid), yes)\n$(error Invalid configuration $(CONFIG))\nendif\n");
			return string;
		}

	   private:
		const std::unordered_map<std::string, projgen::Project::Configuration>& configs;
	};

	// a general product rule
	struct MakefileProductRule : public MakefileGeneratable {
		std::string product;
		std::string consumeDeps;

		MakefileProductRule(const std::string& product, const std::string& consumeDeps) : product(product), consumeDeps(consumeDeps) {}

		std::string Generate() override { return std::format("{}: {}\n\t{}", product, consumeDeps, GenerateRuleCommand()); }

	   private:
		virtual std::string GenerateRuleCommand() = 0;
	};

	struct MakefileAllRule : MakefileGeneratable {
		std::string product;
		std::string consumeDeps;

		MakefileAllRule() : product("all"), consumeDeps("obj/$(CONFIG)/ $(NAME)_$(CONFIG).bin") {}

		std::string Generate() override { return std::format("{}: {}\n", product, consumeDeps); }
	};

	struct MakefileCleanRule : MakefileProductRule {
		MakefileCleanRule() : MakefileProductRule("clean", "") {}

	   private:
		std::string GenerateRuleCommand() override { return "rm -rf $(NAME)_$(CONFIG).elf $(NAME)_$(CONFIG).bin obj/$(CONFIG)"; }
	};

	struct MakefileObjDirRule : MakefileProductRule {
		MakefileObjDirRule() : MakefileProductRule("obj/$(CONFIG)/", "") {}

	   private:
		std::string GenerateRuleCommand() override { return "mkdir -p $@"; }
	};

	// a pattern rule
	// e.g "%.o: %.c" or such
	struct MakefilePatternRule : public MakefileGeneratable {
		std::string productExtension;
		std::string consumeExtension;

		MakefilePatternRule(const std::string& productExtension, const std::string& consumeExtension)
			: productExtension(productExtension), consumeExtension(consumeExtension) {}

		std::string Generate() override { return std::format("{}: {}\n\t{}", productExtension, consumeExtension, GenerateRuleCommand()); }

	   private:
		virtual std::string GenerateRuleCommand() = 0;
	};

	struct MakefileAsmRule : public MakefilePatternRule {
		MakefileAsmRule() : MakefilePatternRule("obj/$(CONFIG)/%.o", "%.S") {}

	   private:
		std::string GenerateRuleCommand() override { return "$(CC) -xassembler-with-cpp -c $($(CONFIG)_CCFLAGS) $< -o $@"; }
	};

	struct MakefileCRule : public MakefilePatternRule {
		MakefileCRule() : MakefilePatternRule("obj/$(CONFIG)/%.o", "%.c") {}

	   private:
		std::string GenerateRuleCommand() override { return "$(CC) -c $($(CONFIG)_CCFLAGS) $< -o $@"; }
	};

	struct MakefileCXXRule : public MakefilePatternRule {
		MakefileCXXRule() : MakefilePatternRule("obj/$(CONFIG)/%.o", "%.cpp") {}

	   private:
		std::string GenerateRuleCommand() override { return "$(CXX) -c $($(CONFIG)_CXXFLAGS) $< -o $@"; }
	};

	struct MakefileLinkRule : public MakefileProductRule {
		MakefileLinkRule() : MakefileProductRule("$(NAME)_$(CONFIG).elf", "$(OBJS)") {};

	   private:
		std::string GenerateRuleCommand() override { return "$(LD) $($(CONFIG)_LDFLAGS) $(OBJS) -o $@"; }
	};

	struct MakefileFlatBinaryRule : MakefileProductRule {
		MakefileFlatBinaryRule() : MakefileProductRule("$(NAME)_$(CONFIG).bin", "$(NAME)_$(CONFIG).elf") {};

	   private:
		std::string GenerateRuleCommand() override { return "$(OBJCOPY) $^ -O binary $@"; }
	};

	struct MakefileWriter {
		MakefileWriter(const fs::path& path) {
			file = projgen::util::UniqueFopen((path / "Makefile").string(), "w");
			LUCORE_CHECK(file, "Could not open {} for writing", (path / "Makefile").string());
		}

		bool Write(const std::vector<std::unique_ptr<MakefileGeneratable>>& g) {
			for(auto& p : g) {
				WriteLine("# Generated by LCPU project generator\n");
				WriteLine("# Do not modify this file.\n");

				auto generated_data = p->Generate();
				if(std::fwrite(generated_data.data(), 1, generated_data.length(), file.get()) != generated_data.length())
					return false;

				if(!fputc('\n', file.get()))
					return false;
			}

			return true;
		}

	   private:

		void WriteLine(std::string_view line) {
			std::fwrite(line.data(), sizeof(char), line.length(), file.get());
		}

		projgen::util::UniqueFilePtr file;
	};

} // namespace projgen::make
