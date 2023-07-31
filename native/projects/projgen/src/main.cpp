//! Main for the LCPU project generator

#include <filesystem>
#include <lucore/Assert.hpp>
#include <lucore/Logger.hpp>
#include <lucore/StdoutSink.hpp>
#include <unordered_map>

#include "BaseConfig.hpp"
#include "FsUtils.hpp"
#include "Makefile.hpp"
#include "Project.hpp"

// TODO:
// Once there's better C++23 ranges support, this can/should be replaced with:
// (objects | views::join_with(' ')
//          | std::ranges::to<std::string>())
// I want better ranges support in gcc and clang, this already works in MSVC :(
inline auto join(const std::vector<std::string>& values, const std::string_view seperator = " ") {
	auto string = std::string {};
	for(auto& value : values)
		string += std::format("{}{}", value, seperator);
	return string;
}

int main(int argc, char** argv) {
	lucore::LoggerAttachStdout();
	lucore::LogInfo("LCPU project generator");

	auto project_json_path = (fs::current_path() / "project.json");

	if(!fs::exists(project_json_path)) {
		lucore::LogFatal("The directory \"{}\" does not seem like it's a LCPU project to me", fs::current_path().string());
		return 1;
	}

	try {
		auto project = projgen::util::ParseJsonFromFile<projgen::Project>(project_json_path);

		lucore::LogInfo("Generating Makefile for project \"{}\".", project.name);

		auto sourceFiles = projgen::SourceFile::MakeArray(fs::current_path(), project.sourceFileNames);
		auto objects = std::vector<std::string> {};
		bool foundLdScript = false;

		for(auto& source : sourceFiles) {
			LUCORE_CHECK(source.type != projgen::SourceFile::Type::Invalid,
						 "Source file {} with Invalid type in source files. Refusing to generate project", source.path.string());

			if(source.type == projgen::SourceFile::Type::LinkerScript) {
				LUCORE_CHECK(!foundLdScript, "Project invalid; has more than 1 .ld script file");
				foundLdScript = true;
				for(auto& kv : project.configurations) {
					if(kv.second.linkerFlags.has_value()) {
						kv.second.linkerFlags = std::format("-T {} {}", source.path.string(), *kv.second.linkerFlags);
					} else {
						kv.second.linkerFlags = std::format("-T {}", source.path.string());
					}
				}
				continue;
			}

			objects.push_back(std::format("obj/$(CONFIG)/{}", source.path.replace_extension(".o").string()));
		}

		projgen::make::MakefileWriter writer(fs::current_path());

		auto generators = std::vector<std::unique_ptr<projgen::make::MakefileGeneratable>> {};
		generators.emplace_back(new projgen::make::MakefileGlobalVariables(project.name, join(objects)));
		generators.emplace_back(new projgen::make::MakefileConfiguration(project.configurations));
		generators.emplace_back(new projgen::make::MakefileAllRule());
		generators.emplace_back(new projgen::make::MakefileCleanRule());
		generators.emplace_back(new projgen::make::MakefileObjDirRule());
		generators.emplace_back(new projgen::make::MakefileAsmRule());
		generators.emplace_back(new projgen::make::MakefileCRule());
		generators.emplace_back(new projgen::make::MakefileCXXRule());
		generators.emplace_back(new projgen::make::MakefileFlatBinaryRule());
		generators.emplace_back(new projgen::make::MakefileLinkRule());

		if(!writer.Write(generators)) {
			lucore::LogError("Could not generate project");
			return 1;
		} else {
			lucore::LogInfo("Generated Makefile \"{}\".", (fs::current_path() / "Makefile").string());
		}

	} catch(daw::json::json_exception& ex) {
		lucore::LogFatal("Exception while trying to parse JSON data: {} {}", ex.what(), ex.reason());
	}

	return 0;
}
