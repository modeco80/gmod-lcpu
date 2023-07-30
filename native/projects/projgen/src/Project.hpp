#include <daw/json/daw_json_link.h>

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

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
		using type =
		json_member_list<json_string<"Name">,
						 json_key_value<"Configurations", std::unordered_map<std::string, projgen::Project::Configuration>, projgen::Project::Configuration>,
						 json_array<"Sources", std::string> >;

		static inline auto to_json_data(const projgen::Project& value) {
			return std::forward_as_tuple(value.name, value.configurations, value.sourceFileNames);
		}
	};

} // namespace daw::json
