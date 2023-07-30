#include <cstdio>
#include <filesystem>
#include <memory>
#include <string>

namespace fs = std::filesystem;

namespace projgen::util {

	using unique_file_ptr = std::unique_ptr<std::FILE, decltype(&std::fclose)>;

	unique_file_ptr UniqueFopen(std::string_view path, std::string_view mode) {
		return unique_file_ptr(std::fopen(path.data(), mode.data()), &std::fclose);
	}

	std::string ReadFileAsString(const fs::path& path) {
		auto file = UniqueFopen(path.string(), "r");
		std::string data;
		if(file) {
			std::fseek(file.get(), 0, SEEK_END);
			auto len = std::ftell(file.get());
			std::fseek(file.get(), 0, SEEK_SET);

			data.resize(len);
			std::fread(data.data(), 1, len, file.get());
		}
		return data;
	}

} // namespace projgen::util
