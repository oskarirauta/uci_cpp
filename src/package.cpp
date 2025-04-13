#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <unistd.h>

#include "uci/util.hpp"
#include "uci.hpp"

UCI::TYPE& UCI::PACKAGE::operator [](const std::string& type) {

	std::string name = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(type))));

	if ( auto it = std::find_if(this -> _types.begin(), this -> _types.end(), [&name](const UCI::TYPE& t) {
			return t._name == name;
		}); it != this -> _types.end())
		return *it;

	throw std::runtime_error("type " + name + " is out of bounds, type not found");
}

UCI::TYPE& UCI::PACKAGE::operator [](size_t index) {

	if ( this -> _types.empty())
		throw std::runtime_error("type at index " + std::to_string(index) + " is out of bounds, package is empty");

	if ( index >= this -> _types.size())
		throw std::runtime_error("type at index " + std::to_string(index) + " is out of bounds, last available index is " + std::to_string(this -> _types.size() - 1 ));

	auto it = this -> _types.begin();
	if ( index > 0 )
		std::advance(it, index);

	return *it;
}

const UCI::TYPE& UCI::PACKAGE::operator [](const std::string& type) const {

	std::string name = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(type))));

	if ( auto it = std::find_if(this -> _types.begin(), this -> _types.end(), [&name](const UCI::TYPE& t) {
			return t._name == name;
		}); it != this -> _types.end())
		return *it;

	throw std::runtime_error("type " + name + " is out of bounds, type not found");
}

const UCI::TYPE& UCI::PACKAGE::operator [](size_t index) const {

	if ( this -> _types.empty())
		throw std::runtime_error("type at index " + std::to_string(index) + " is out of bounds, package is empty");

	if ( index >= this -> _types.size())
		throw std::runtime_error("type at index " + std::to_string(index) + " is out of bounds, last available index is " + std::to_string(this -> _types.size() - 1 ));

	auto it = this -> _types.begin();
	if ( index > 0 )
		std::advance(it, index);

	return *it;
}

UCI::PACKAGE::iterator UCI::PACKAGE::begin() {
	return this -> _types.begin();
}

UCI::PACKAGE::iterator UCI::PACKAGE::end() {
	return this -> _types.end();
}

UCI::PACKAGE::const_iterator UCI::PACKAGE::begin() const {
	return this -> _types.cbegin();
}

UCI::PACKAGE::const_iterator UCI::PACKAGE::end() const {
	return this -> _types.cend();
}

UCI::PACKAGE::const_iterator UCI::PACKAGE::cbegin() const {
	return this -> _types.cbegin();
}

UCI::PACKAGE::const_iterator UCI::PACKAGE::cend() const {
	return this -> _types.cend();
}

size_t UCI::PACKAGE::size() const {
	return this -> _types.size();
}

bool UCI::PACKAGE::empty() const {
	return this -> _types.empty();
}

bool UCI::PACKAGE::contains(const std::string& type) const {

	std::string name = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(type))));

	return std::find_if(this -> _types.begin(), this -> _types.end(), [&name](const UCI::TYPE& t) { return t._name == name; }) != this -> _types.end();
}

std::string UCI::PACKAGE::package() const {

	std::string s(this -> _package);

	for ( auto pos = s.find_first_of('/'); pos != std::string::npos; pos = s.find_first_of('/'))
		s.erase(0, pos + 1);

	return s;
}

std::string UCI::PACKAGE::filename() const {
	return this -> _package;
}

void UCI::PACKAGE::set_filename(const std::string& package) {

	std::string _package = UCI::STR::trim(package);

	if ( _package.empty()) {

		this -> _package = "";
	} else {

		if ( _package.find_first_of('/') == std::string::npos )
			this -> _package = "/etc/config/" + _package;
		else this -> _package = _package;
	}
}

UCI::SECTION& UCI::PACKAGE::add(const std::string& type, const std::optional<std::string>& section) {

	std::string type_name = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(type))));
	std::string section_name = section == std::nullopt ? "" : UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(*section))));

	auto _type = std::find_if(this -> _types.begin(), this -> _types.end(), [&type_name](const UCI::TYPE& t) { return type_name == t._name; });
	if ( _type == this -> _types.end()) {
		this -> _types.push_back(UCI::TYPE(this, UCI::TYPE::next_id(true), type_name));
		for ( auto it = this -> _types.begin(); it != this -> _types.end(); it++ )
			_type = it;
	}

	try {
		if ( section_name.empty())
			return _type -> add(std::nullopt);
		else return _type -> add(section_name);

	} catch ( const std::runtime_error& e ) {
		throw std::runtime_error("cannot add " + type_name + " section " + ( section_name.empty() ? "" : ( section_name + " " )) + ", " + std::string(e.what()));
	}
}

UCI::SECTION& UCI::PACKAGE::add(const std::string& type) {

	std::string type_name = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(type))));

	auto _type = std::find_if(this -> _types.begin(), this -> _types.end(), [&type_name](const UCI::TYPE& t) { return type_name == t._name; });
	if ( _type == this -> _types.end()) {
		this -> _types.push_back(UCI::TYPE(this, UCI::TYPE::next_id(true), type_name));
		for ( auto it = this -> _types.begin(); it != this -> _types.end(); it++ )
			_type = it;
	}

	try {
		return _type -> add(std::nullopt);
	} catch ( const std::runtime_error& e ) {
		throw std::runtime_error("cannot add " + type_name + " section, " + std::string(e.what()));
	}
}

size_t UCI::PACKAGE::index_of(const UCI::TYPE& type) const {

	auto it = std::find_if(this -> _types.begin(), this -> _types.end(), [&type](const UCI::TYPE& t) { return type._id == t._id; });
	return it == this -> _types.end() ? -1 : std::distance(this -> _types.begin(), it);
}

UCI::TYPE& UCI::PACKAGE::get_type(const long& type_id) {

	auto it = std::find_if(this -> _types.begin(), this -> _types.end(), [&type_id](const UCI::TYPE& t ) { return type_id == t._id; });
	return *it;
}

void UCI::PACKAGE::remove(const long& type_id, const long& section_id) {

	if ( auto it = std::find_if(this -> _types.begin(), this -> _types.end(), [&type_id](const UCI::TYPE& t) {
			return type_id == t._id;
		}); it != this -> _types.end()) {

		it -> _sections.erase(std::remove_if(it -> _sections.begin(), it -> _sections.end(), [&section_id](const UCI::SECTION& s) {
			return section_id == s._id;
		}), it -> _sections.end());

		this -> clean_up();
	}
}

void UCI::PACKAGE::clean_up() {

	for ( auto it = this -> _types.begin(); it != this -> _types.end(); it++ ) {

		it -> _sections.erase(std::remove_if(it -> _sections.begin(), it -> _sections.end(), [](const UCI::SECTION& s) {
			return s._options.empty();
		}), it -> _sections.end());
	}

	this -> _types.erase(std::remove_if(this -> _types.begin(), this -> _types.end(), [](const UCI::TYPE& t) {
		return t._sections.empty();
	}), this -> _types.end());
}

UCI::PACKAGE::PACKAGE(const std::string& package) {

	std::string _package = UCI::STR::trim(package);

	if ( _package.empty())
		this -> _package = "";
	else {

		if ( _package.find_first_of('/') == std::string::npos )
			this -> _package = "/etc/config/" + _package;
		else this -> _package = _package;

		if ( ::access(this -> _package.c_str(), F_OK) != 0 )
			throw std::runtime_error("cannot read UCI package " + _package + "(" + this -> _package + "), file does not exist");

		std::ifstream f(this -> _package);
		if ( !f.good() || f.bad() || f.fail()) {

			if ( f.is_open())
				f.close();

			throw std::runtime_error("cannot read UCI package " + package + ", file read error");
		}

		std::stringstream ss;
		std::string blob;

		ss << f.rdbuf();
		blob = ss.str();
		f.close();

		try {
			this -> parse(blob);
		} catch ( const std::runtime_error& e ) {
			throw std::runtime_error("error while parsing uci package " + this -> package() + ", " + std::string(e.what()));
		}
	}

}

UCI::PACKAGE::PACKAGE() {

	this -> _package = "";
}
