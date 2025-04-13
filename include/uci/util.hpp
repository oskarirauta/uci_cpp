#pragma once
#include <string>
#include <vector>
#include <optional>

namespace UCI {

	namespace STR {

		std::string to_lower(const std::string& str);
		std::string to_upper(const std::string& str);

		std::string trim(const std::string& str, const std::string& whitespace);
		std::string trim(const std::string& str);
		std::string unquoted(const std::string& str, bool trimmed = false);
	}

}
