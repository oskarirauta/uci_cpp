#include <cerrno>
#include <cstring>
#include "uci/util.hpp"

const std::string _ws = " \t\n\r\f\v";

std::string UCI::STR::to_lower(const std::string& str) {

	std::string _str(str);

        for ( auto& ch : _str )
                if ( std::isupper(ch))
                        ch ^= 32;
        return _str;
}

std::string UCI::STR::to_upper(const std::string& str) {

	std::string _str(str);

        for ( auto& ch : _str )
                if ( std::islower(ch))
			ch &= ~32;

        return _str;
}

std::string UCI::STR::unquoted(const std::string& str, bool trimmed) {

	std::string _str(UCI::STR::trim(str));

	if ( _str.empty())
		return _str;

	int begin = -1, end = -1;
	std::string ws = " \t\r\f\v";

	for ( size_t idx = 0; idx < _str.size() && begin < 0; idx++ )
		if ( ws.find_first_of(_str.at(idx)) == std::string::npos && (
			_str.at(idx) == '\'' || _str.at(idx) == '\"' ))
			begin = (int)idx;

	if ( begin < 0 )
		return str;

	for ( size_t idx = _str.size() - 1; idx > (size_t)begin && end < 0; idx-- )
		if ( ws.find_first_of(_str.at(idx)) == std::string::npos &&
			_str.at(idx) == _str.at(begin))
			end = (int)idx;

	if ( end > begin )
		_str = _str.substr(begin + 1, end - ( begin + 1));

	return end > begin ? ( trimmed ? UCI::STR::trim(_str) : _str ) : str;
}

std::string UCI::STR::trim(const std::string& str, const std::string& whitespace) {

	if ( whitespace.empty())
		return str;

	std::string _str(str);

	_str.erase(_str.find_last_not_of(whitespace) + 1);
	_str.erase(0, _str.find_first_not_of(whitespace));

	return _str;
}

std::string UCI::STR::trim(const std::string& str) {

	return UCI::STR::trim(str, _ws);
}
