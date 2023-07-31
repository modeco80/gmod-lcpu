#pragma once

#include <daw/json/daw_json_link.h>

#include <cstdio>
#include <filesystem>
#include <memory>
#include <string>

namespace fs = std::filesystem;

namespace projgen::util {

	using UniqueFilePtr = std::unique_ptr<std::FILE, decltype(&std::fclose)>;

	inline UniqueFilePtr UniqueFopen(std::string_view path, std::string_view mode) {
		return UniqueFilePtr(std::fopen(path.data(), mode.data()), &std::fclose);
	}

	inline std::string ReadFileAsString(const fs::path& path) {
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

	template <class T>
	inline T ParseJsonFromFile(const fs::path& path) {
		auto data = projgen::util::ReadFileAsString(path);
		return daw::json::from_json<T>(data);
	}

} // namespace projgen::util
