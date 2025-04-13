#include <algorithm>
#include <fstream>

#include "uci/util.hpp"
#include "uci.hpp"

static const std::string whitespace = " \t\n\r\f\v";
static const std::string spaces = " \t\r\f\v";

static size_t len(const std::string& s) {
	return s.size() - 1;
}

static void consume_ws(const std::string& s, size_t& pos) {

	while ( pos < len(s) && std::isspace(s.at(pos)))
		pos++;
}

static void consume_space(const std::string& s, size_t& pos) {

	while ( pos < len(s) && spaces.find_first_of(s.at(pos)) != std::string::npos )
		pos++;
}

static std::string parse_word(const std::string& s, size_t& pos) {

	std::string word;
	std::string::value_type quote = 0;

	consume_ws(s, pos);

	if ( s.at(pos) == '\'' || s.at(pos) == '"' )
		quote = s.at(pos++);

	while ( pos < len(s)) {

		if ( quote != 0 && s.at(pos) == quote && word.back() != '\\' ) {
			pos++;
			break;
		} else if ( quote != 0 ) {
			if ( s.at(pos) == '\n' )
				throw std::runtime_error("line-feed inside quotes");
			word += s.at(pos++);
			continue;
		}

		if ( s.at(pos) == '#' ) { // comment
			while ( pos < len(s) && s.at(pos) != '\n' )
				pos++;
			break;
		}

		if ( whitespace.find_first_of(s.at(pos)) != std::string::npos )
			break;

		word += s.at(pos++);
	}

	return quote == 0 ? word : ( "'" + word + "'" );
}

static bool is_numeric(const std::string& s) {

	std::string _s = UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(s)));

	if ( _s.front() == '-' )
		_s.erase(0, 1);

	if ( _s.back() == '.' )
		_s.pop_back();

	for ( auto& ch : _s )
		if ( !std::isdigit(ch))
			return false;

	return true;
}

static bool is_floating(const std::string& s) {

	bool has_dec_point = false;
	std::string _s = UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(s)));

	if ( _s.front() == '-' )
		_s.erase(0, 1);

	if ( _s.empty())
		return false;

	for ( auto& ch : _s ) {

		if ( ch == '.' && !has_dec_point )
			has_dec_point = true;
		else if ( ch == '.' )
			return false;
		else if ( !std::isdigit(ch))
			return false;
	}

	return true;
}

static long double to_float(const std::string& s) {

	std::string _s = UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(s)));
	bool negative = false;

	if ( _s.front() == '-' ) {
		negative = true;
		_s.erase(0, 1);
	}

	if ( _s.front() == '.' )
		_s = "0" + _s;

	if ( _s.back() == '.' )
		_s.pop_back();

	try {
		long double ld = std::stold(_s);
		return negative ? -ld : ld;
	} catch ( ... ) {
		throw std::runtime_error("failed to convert " + _s + " to float");
	}
}

UCI::OPTION to_option(const std::string& value) {

	std::string _s = UCI::STR::unquoted(UCI::STR::trim(value));

	if ( is_numeric(_s)) {

		try {
			long long ll = std::stoll(_s);
			return UCI::OPTION(ll);
		} catch ( ... ) {
			return UCI::OPTION(value);
		}

	} else if ( is_floating(_s)) {

		try {
			long double ld = to_float(_s);
			return UCI::OPTION(ld);
		} catch ( ... ) {
			return UCI::OPTION(_s);
		}
	}

	return UCI::OPTION(value);
}

void UCI::PACKAGE::parse(const std::string& blob) {

	size_t pos = 0;
	std::string word = "";
	std::string word1 = "";
	std::string word2 = "";
	std::string word3 = "";
	int wc = 0;

	std::string type = "";
	std::string section = "";

	size_t section_index = -1;
	this -> _types.clear();

	while ( pos < len(blob)) {

		try {
			word = parse_word(blob, pos);
		} catch ( const std::runtime_error& e ) {
			throw e;
		}

		consume_space(blob, pos);
		wc++;

		if ( wc == 2 && UCI::STR::to_lower(word1) == "config" && pos < len(blob) && blob.at(pos) == '\n' ) {

			word2 = word;
			wc++;

		} else {

			if (( wc == 1 || wc == 2 ) && pos < len(blob) &&
				blob.at(pos) == '\n' )
				throw std::runtime_error("parse error, abnormal position for line feed");

			if ( wc == 1 ) word1 = word;
			else if ( wc == 2 ) word2 = word;
			else if ( wc == 3 ) word3 = word;
		}

		if ( wc != 3 )
			continue;

		consume_space(blob, pos);
		if ( pos < len(blob) && blob.at(pos) != '\n' && blob.at(pos) != '#' )
			throw std::runtime_error("syntax error");

		word1 = UCI::STR::trim(UCI::STR::to_lower(word1));
		word2 = UCI::STR::trim(UCI::STR::unquoted(UCI::STR::to_lower(word2)));

		if ( word1.find_first_not_of("1234567890abcdefghijklmnopqrstuvwxyz_") != std::string::npos )
			throw std::runtime_error("invalid characters found in \"" + word1 + "\"");

		if ( word2.find_first_not_of("1234567890abcdefghijklmnopqrstuvwxyz_") != std::string::npos )
			throw std::runtime_error("invalid characters in " + std::string( word1 == "config" ? "type" : "identifier" ) + " \"" + word2 + "\"");

		if ( word1 == "config" ) {

			type = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(word2))));
			section = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(word3))));
			section_index = -1;

			if ( type.empty())
				throw std::runtime_error("invalid section type, type cannot be empty");

			if ( !this -> contains(type)) {

				if ( section.empty())
					section_index = this -> add(type).index();
				else section_index = this -> add(type, section).index();

			} else if ( !section.empty() && !this -> operator[](type).contains(section)) {
				section_index = this -> add(type, section).index();
			} else if ( section.empty()) {
				section_index = this -> add(type).index();
			} else throw std::runtime_error("duplicate " + type + " section " + section);

			word1 = word2 = word3 = "";
			wc = 0;

			continue;
		}

		if ( word1 != "option" && word1 != "list" )
			throw std::runtime_error("syntax error, invalid directive " + word1);

		if ( section_index == (size_t)-1 )
			throw std::runtime_error("syntax error, " + word1 + " directive before section definition");

		std::string option = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(word2))));
		std::string value = UCI::STR::trim(UCI::STR::unquoted(word3));

		if ( word1 == "option" ) {

			if ( this -> operator [](type)[section_index].contains(option))
				throw std::runtime_error("syntax error, duplicate option " + option);

			this -> operator [](type)[section_index].add(option, to_option(value));

		} else if ( word1 == "list" ) {

			if ( this -> operator [](type)[section_index].contains(option)) {

				if ( this -> operator[](type)[section_index][option].type() != UCI::TYPES::ARRAY )
					throw std::runtime_error("invalid directive, option " + option + " exists and is not a list");

				this -> operator[](type)[section_index][option].add(to_option(value));

			} else {

				this -> operator[](type)[section_index].add(option, std::vector<UCI::OPTION>{ to_option(value) });
			}
		}

		word1 = word2 = word3 = "";
		wc = 0;
	}

	this -> clean_up();
}

std::string UCI::PACKAGE::filedata() const {

	std::string s = "";

	for ( auto type = this -> _types.begin(); type != this -> _types.end(); type++ ) {

		for ( auto section = type -> _sections.begin(); section != type -> _sections.end(); section++ ) {

			s += ( !s.empty() ? "\n\n" : "" ) + std::string("config ") + type -> _name;

			if ( section -> _name != std::nullopt )
				s += " '" + *section -> _name + "'";

			for ( auto option = section -> _options.begin(); option != section -> _options.end(); option++ ) {

				if ( option -> type() != UCI::TYPES::ARRAY ) {

					s += "\n\toption " + option -> _name + " ";

					if ( option -> type() == UCI::TYPES::STRING )
						s += "'" + option -> to_string() + "'";
					else s += option -> to_string();

				} else {

					for ( auto child = option -> begin(); child != option -> end(); child++ ) {

						s += "\n\tlist " + option -> _name + " ";
						if ( child -> type() == UCI::TYPES::STRING )
							s += "'" + child -> to_string() + "'";
						else s += child -> to_string();
					}

				}
			}

		}

	}

	return s;
}

void UCI::PACKAGE::save() const {

	std::string content = this -> filedata();

	std::ofstream f(this -> _package, std::ios::trunc);
	f << content << std::endl;
	f.close();
}

void UCI::PACKAGE::save() {

	this -> clean_up();
	std::string content = this -> filedata();

	std::ofstream f(this -> _package, std::ios::trunc);
	f << content << std::endl;
	f.close();
}
