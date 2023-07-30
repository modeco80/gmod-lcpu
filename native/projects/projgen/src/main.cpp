//! Main for the LCPU project generator

#include <lucore/Logger.hpp>
#include <lucore/StdoutSink.hpp>
#include <unordered_map>

#include "BaseConfig.hpp"
#include "FsUtils.hpp"
#include "Project.hpp"

template <class T>
T ParseJsonFromFile(const fs::path& path) {
	auto data = projgen::util::ReadFileAsString(path);
	return daw::json::from_json<T>(data);
}

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
		if(extension == "s" || extension == "S")
			return Type::AsmSourceFile;

		if(extension == "c")
			return Type::CSourceFile;
		if(extension == "cpp" || extension == "cc")
			return Type::CppSourceFile;
		if(extension == "ld")
			return Type::LinkerScript;

		return Type::Invalid;
	}

	explicit SourceFile(const std::string& filename) : filename(filename) {
		if(auto pos = filename.rfind('.'); pos != std::string::npos) {
			type = TypeFromExtension(filename.substr(pos + 1));
		} else {
			type = Type::Invalid;
		}
	}

	static std::vector<SourceFile> MakeArray(const std::vector<std::string>& filenames) {
		auto vec = std::vector<SourceFile>();
		vec.reserve(filenames.size());

		for(auto& filename : filenames)
			vec.emplace_back(filename);

		return vec;
	}

	Type type;
	std::string filename;
};

int main(int argc, char** argv) {
	lucore::LoggerAttachStdout();

	lucore::LogInfo("LCPU project generator!");

	auto project_json_path = (fs::current_path() / "project.json");

	if(!fs::exists(project_json_path)) {
		lucore::LogFatal("The directory \"{}\" does not seem like it's a project to me", fs::current_path().string());
		return 1;
	}

	try {
		auto project = ParseJsonFromFile<projgen::Project>(project_json_path);

		for(auto& pair : project.configurations) {
			std::printf("%s: %s %s %s\n", pair.first.c_str(), pair.second.cCompileFlags.c_str(), pair.second.cppCompileFlags.c_str(),
						pair.second.linkerFlags.value_or(PROJGEN_BASE_LD_FLAGS).c_str());
		}

		auto sourceFiles = SourceFile::MakeArray(project.sourceFileNames);

		for(auto& source : sourceFiles) {
			std::printf("%s -> %d\n", source.filename.c_str(), source.type);
		}

	} catch(daw::json::json_exception& ex) {
		lucore::LogFatal("Exception while trying to parse JSON data: {} {}", ex.what(), ex.reason());
	}

	return 0;
}
