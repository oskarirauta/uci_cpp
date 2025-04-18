#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <unistd.h>

#include "uci/util.hpp"
#include "uci.hpp"

UCI::CATEGORY& UCI::PACKAGE::operator [](const std::string& category) {

	std::string name = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(category))));

	if ( auto it = std::find_if(this -> _categories.begin(), this -> _categories.end(), [&name](const UCI::CATEGORY& t) {
			return t._name == name;
		}); it != this -> _categories.end())
		return *it;

	throw std::runtime_error("category " + name + " is out of bounds, category not found");
}

UCI::CATEGORY& UCI::PACKAGE::operator [](size_t index) {

	if ( this -> _categories.empty())
		throw std::runtime_error("category at index " + std::to_string(index) + " is out of bounds, package is empty");

	if ( index >= this -> _categories.size())
		throw std::runtime_error("category at index " + std::to_string(index) + " is out of bounds, last available index is " + std::to_string(this -> _categories.size() - 1 ));

	auto it = this -> _categories.begin();
	if ( index > 0 )
		std::advance(it, index);

	return *it;
}

const UCI::CATEGORY& UCI::PACKAGE::operator [](const std::string& category) const {

	std::string name = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(category))));

	if ( auto it = std::find_if(this -> _categories.begin(), this -> _categories.end(), [&name](const UCI::CATEGORY& t) {
			return t._name == name;
		}); it != this -> _categories.end())
		return *it;

	throw std::runtime_error("category " + name + " is out of bounds, category not found");
}

const UCI::CATEGORY& UCI::PACKAGE::operator [](size_t index) const {

	if ( this -> _categories.empty())
		throw std::runtime_error("category at index " + std::to_string(index) + " is out of bounds, package is empty");

	if ( index >= this -> _categories.size())
		throw std::runtime_error("category at index " + std::to_string(index) + " is out of bounds, last available index is " + std::to_string(this -> _categories.size() - 1 ));

	auto it = this -> _categories.begin();
	if ( index > 0 )
		std::advance(it, index);

	return *it;
}

UCI::PACKAGE::iterator UCI::PACKAGE::begin() {
	return this -> _categories.begin();
}

UCI::PACKAGE::iterator UCI::PACKAGE::end() {
	return this -> _categories.end();
}

UCI::PACKAGE::const_iterator UCI::PACKAGE::begin() const {
	return this -> _categories.cbegin();
}

UCI::PACKAGE::const_iterator UCI::PACKAGE::end() const {
	return this -> _categories.cend();
}

UCI::PACKAGE::const_iterator UCI::PACKAGE::cbegin() const {
	return this -> _categories.cbegin();
}

UCI::PACKAGE::const_iterator UCI::PACKAGE::cend() const {
	return this -> _categories.cend();
}

size_t UCI::PACKAGE::size() const {
	return this -> _categories.size();
}

bool UCI::PACKAGE::empty() const {
	return this -> _categories.empty();
}

bool UCI::PACKAGE::contains(const std::string& category) const {

	std::string name = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(category))));

	return std::find_if(this -> _categories.begin(), this -> _categories.end(), [&name](const UCI::CATEGORY& t) { return t._name == name; }) != this -> _categories.end();
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

UCI::SECTION& UCI::PACKAGE::add(const std::string& category, const std::optional<std::string>& section) {

	std::string category_name = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(category))));
	std::string section_name = section == std::nullopt ? "" : UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(*section))));

	auto _category = std::find_if(this -> _categories.begin(), this -> _categories.end(), [&category_name](const UCI::CATEGORY& t) { return category_name == t._name; });
	if ( _category == this -> _categories.end()) {
		this -> _categories.push_back(UCI::CATEGORY(this, UCI::CATEGORY::next_id(true), category_name));
		for ( auto it = this -> _categories.begin(); it != this -> _categories.end(); it++ )
			_category = it;
	}

	try {
		if ( section_name.empty())
			return _category -> add(std::nullopt);
		else return _category -> add(section_name);

	} catch ( const std::runtime_error& e ) {
		throw std::runtime_error("cannot add " + category_name + " section " + ( section_name.empty() ? "" : ( section_name + " " )) + ", " + std::string(e.what()));
	}
}

UCI::SECTION& UCI::PACKAGE::add(const std::string& category) {

	std::string category_name = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(category))));

	auto _category = std::find_if(this -> _categories.begin(), this -> _categories.end(), [&category_name](const UCI::CATEGORY& t) { return category_name == t._name; });
	if ( _category == this -> _categories.end()) {
		this -> _categories.push_back(UCI::CATEGORY(this, UCI::CATEGORY::next_id(true), category_name));
		for ( auto it = this -> _categories.begin(); it != this -> _categories.end(); it++ )
			_category = it;
	}

	try {
		return _category -> add(std::nullopt);
	} catch ( const std::runtime_error& e ) {
		throw std::runtime_error("cannot add " + category_name + " section, " + std::string(e.what()));
	}
}

size_t UCI::PACKAGE::index_of(const UCI::CATEGORY& category) const {

	auto it = std::find_if(this -> _categories.begin(), this -> _categories.end(), [&category](const UCI::CATEGORY& t) { return category._id == t._id; });
	return it == this -> _categories.end() ? -1 : std::distance(this -> _categories.begin(), it);
}

UCI::CATEGORY& UCI::PACKAGE::get_category(const long& category_id) {

	auto it = std::find_if(this -> _categories.begin(), this -> _categories.end(), [&category_id](const UCI::CATEGORY& t ) { return category_id == t._id; });
	return *it;
}

void UCI::PACKAGE::remove(const long& category_id, const long& section_id) {

	if ( auto it = std::find_if(this -> _categories.begin(), this -> _categories.end(), [&category_id](const UCI::CATEGORY& t) {
			return category_id == t._id;
		}); it != this -> _categories.end()) {

		it -> _sections.erase(std::remove_if(it -> _sections.begin(), it -> _sections.end(), [&section_id](const UCI::SECTION& s) {
			return section_id == s._id;
		}), it -> _sections.end());

		this -> clean_up();
	}
}

void UCI::PACKAGE::clean_up() {

	for ( auto it = this -> _categories.begin(); it != this -> _categories.end(); it++ ) {

		it -> _sections.erase(std::remove_if(it -> _sections.begin(), it -> _sections.end(), [](const UCI::SECTION& s) {
			return s._options.empty();
		}), it -> _sections.end());
	}

	this -> _categories.erase(std::remove_if(this -> _categories.begin(), this -> _categories.end(), [](const UCI::CATEGORY& t) {
		return t._sections.empty();
	}), this -> _categories.end());
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
