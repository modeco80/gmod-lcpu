#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "FsUtils.hpp"

namespace projgen {
	struct Project {
		struct Configuration {
			/// The C/C++/linker flags for this configuration.
			/// These append to the defaults.
			std::string cCompileFlags;
			std::string cppCompileFlags;
			std::optional<std::string> linkerFlags;
		};

		std::string name;
		std::unordered_map<std::string, Project::Configuration> configurations;
		std::vector<std::string> sourceFileNames;
	};

	// this describes a source file
	struct SourceFile {
		enum class Type {
			Invalid,	   // invalid file
			AsmSourceFile, // Assembly source file
			CSourceFile,   // C source code
			CppSourceFile, // C++ source code
			LinkerScript,  // prepended to linker flags with a -T (only one can exist in a project)
		};

		static constexpr Type TypeFromExtension(std::string_view extension) {
			if(extension == ".S")
				return Type::AsmSourceFile;
			else if(extension == ".c")
				return Type::CSourceFile;
			else if(extension == ".cpp")
				return Type::CppSourceFile;
			else if(extension == ".ld")
				return Type::LinkerScript;
			else
				return Type::Invalid;
		}

		explicit SourceFile(const fs::path& path) {
			if(path.has_extension())
				type = TypeFromExtension(path.extension().string());
			else
				type = Type::Invalid;

			this->path = path.filename();
		}

		static std::vector<SourceFile> MakeArray(const fs::path& sourcePath, const std::vector<std::string>& filenames) {
			auto vec = std::vector<SourceFile>();
			vec.reserve(filenames.size());

			for(auto& filename : filenames)
				vec.emplace_back((sourcePath / filename));

			return vec;
		}

		Type type;
		fs::path path;
	};

} // namespace projgen

/// DAW JSON Link bindings
namespace daw::json {

	template <>
	struct json_data_contract<projgen::Project::Configuration> {
		using type = json_member_list<json_string<"CCompileFlags">, json_string<"CppCompileFlags">, json_string_null<"LinkerFlags"> >;

		static inline auto to_json_data(const projgen::Project::Configuration& value) {
			return std::forward_as_tuple(value.cCompileFlags, value.cppCompileFlags, value.linkerFlags);
		}
	};

	template <>
	struct json_data_contract<projgen::Project> {
		using type = json_member_list<
		json_string<"Name">,
		json_key_value<"Configurations", std::unordered_map<std::string, projgen::Project::Configuration>, projgen::Project::Configuration>,
		json_array<"Sources", std::string> >;

		static inline auto to_json_data(const projgen::Project& value) {
			return std::forward_as_tuple(value.name, value.configurations, value.sourceFileNames);
		}
	};

} // namespace daw::json
