#include <algorithm>
#include <stdexcept>
#include "uci/util.hpp"
#include "uci.hpp"

static long next_section_id = 0;

UCI::OPTION& UCI::SECTION::operator [](const std::string& option) {

	std::string name = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(option))));

	if ( auto it = std::find_if(this -> _options.begin(), this -> _options.end(), [&name](const UCI::OPTION& o) {
			return o._name == name;
		}); it != this -> _options.end())
		return *it;

	throw std::runtime_error("option " + name + " is out of bounds, option not found" +
		( this -> _name == std::nullopt ? "" : ( " in section " + *this -> _name)));
}

UCI::OPTION& UCI::SECTION::operator [](size_t index) {

	if ( this -> _options.empty())
		throw std::runtime_error("option at index " + std::to_string(index) + " is out of bounds, section" +
			( this -> _name == std::nullopt ? "" : ( " " + *this -> _name )) + " is empty");

        if ( index >= this -> _options.size())
		throw std::runtime_error("option at index " + std::to_string(index) + " is out of bounds, last available index in section" +
			( this -> _name == std::nullopt ? "" : ( " " + *this -> _name )) + " is " + std::to_string(this -> _options.size() - 1));

	auto it = this -> _options.begin();
	if ( index > 0 )
		std::advance(it, index);

	return *it;
}

const UCI::OPTION& UCI::SECTION::operator [](const std::string& option) const {

	std::string name = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(option))));

	if ( auto it = std::find_if(this -> _options.begin(), this -> _options.end(), [&name](const UCI::OPTION& o) {
			return o._name == name;
		}); it != this -> _options.end())
		return *it;

	throw std::runtime_error("option " + name + " is out of bounds, option not found" +
		( this -> _name == std::nullopt ? "" : ( " in section " + *this -> _name)));
}

const UCI::OPTION& UCI::SECTION::operator [](size_t index) const {

	if ( this -> _options.empty())
		throw std::runtime_error("option at index " + std::to_string(index) + " is out of bounds, section" +
			( this -> _name == std::nullopt ? "" : ( " " + *this -> _name )) + " is empty");

	if ( index >= this -> _options.size())
		throw std::runtime_error("option at index " + std::to_string(index) + " is out of bounds, last available index in section" +
			( this -> _name == std::nullopt ? "" : ( " " + *this -> _name )) + " is " + std::to_string(this -> _options.size() - 1));

	auto it = this -> _options.begin();
	if ( index > 0 )
		std::advance(it, index);

	return *it;
}

UCI::SECTION::iterator UCI::SECTION::begin() {
	return this -> _options.begin();
}

UCI::SECTION::iterator UCI::SECTION::end() {
	return this -> _options.end();
}

UCI::SECTION::const_iterator UCI::SECTION::begin() const {
	return this -> _options.cbegin();
}

UCI::SECTION::const_iterator UCI::SECTION::end() const {
	return this -> _options.cend();
}

UCI::SECTION::const_iterator UCI::SECTION::cbegin() const {
	return this -> _options.cbegin();
}

UCI::SECTION::const_iterator UCI::SECTION::cend() const {
	return this -> _options.cend();
}

bool UCI::SECTION::is_anonymous() const {
	return this -> _name == std::nullopt;
}

std::string UCI::SECTION::name() const {
	return this -> _name == std::nullopt ? ( "@" + std::to_string(this -> index())) : *this -> _name;
}

size_t UCI::SECTION::index() const {
	return this -> get_category().index_of(*this);
}

unsigned long UCI::SECTION::id() const {
	return this -> _id;
}

size_t UCI::SECTION::size() const {
	return this -> _options.size();
}

bool UCI::SECTION::empty() const {
	return this -> _options.empty();
}

bool UCI::SECTION::contains(const std::string& option) const {

	std::string name = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(option))));

	return std::find_if(this -> _options.begin(), this -> _options.end(), [&name](const UCI::OPTION& o) {
			return name == o._name;
		}) != this -> _options.end();
}

UCI::OPTION& UCI::SECTION::add(const std::string& name, const UCI::OPTION& option) {

	std::string _name = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(name))));

	if ( auto it = std::find_if(this -> _options.begin(), this -> _options.end(), [&_name](const UCI::OPTION& o) {
			return _name == o._name;
		}); it != this -> _options.end()) {

		it -> operator =(option);

		it -> _name = name;
		it -> _package = this -> _package;
		it -> _category_id = this -> _category_id;
		it -> _section_id = this -> _id;
		it -> _parent_id = -1;
		it -> _id = UCI::OPTION::next_id(true);

		if ( it -> type() == UCI::TYPES::ARRAY ) {

			std::vector<UCI::OPTION>& vec = std::get<std::vector<UCI::OPTION>>(it -> _value);
			for ( auto it2 = vec.begin(); it != vec.end(); it++ ) {

				it2 -> _name = _name;
				it2 -> _package = this -> _package;
				it2 -> _category_id = this -> _category_id;
				it2 -> _section_id = this -> _id;
				it2 -> _parent_id = it -> _id;
				it2 -> _id = UCI::OPTION::next_id(true);
			}
		}

		return *it;
	}

	UCI::OPTION new_opt = UCI::OPTION(this -> _package, name, nullptr);

	new_opt._name = name;
	new_opt._value = option._value;
	new_opt._package = this -> _package;
	new_opt._category_id = this -> _category_id;
	new_opt._section_id = this -> _id;
	new_opt._parent_id = -1;
	new_opt._id = UCI::OPTION::next_id(true);

	if ( new_opt.type() == UCI::TYPES::ARRAY ) {

		std::vector<UCI::OPTION>& vec = std::get<std::vector<UCI::OPTION>>(new_opt._value);
		for ( auto it = vec.begin(); it != vec.end(); it++ ) {

			it -> _name = _name;
			it -> _package = this -> _package;
			it -> _category_id = this -> _category_id;
			it -> _section_id = this -> _id;
			it -> _parent_id = new_opt._id;
			it -> _id = UCI::OPTION::next_id(true);
		}
	}

	this -> _options.push_back(new_opt);
	return this -> _options.back();
}

void UCI::SECTION::remove(const UCI::OPTION& option) {

	this -> _options.erase(std::remove_if(this -> _options.begin(), this -> _options.end(), [&option](const UCI::OPTION& o) {
			return option._id == o._id;
		}), this -> _options.end());

	if ( this -> _package != nullptr )
		this -> _package -> clean_up();
}

void UCI::SECTION::remove() {

	if ( this -> _category_id != -1 && this -> _id != -1 )
		this -> _package -> remove(this -> _category_id, this -> _id);
}

size_t UCI::SECTION::index_of(const UCI::OPTION& option) const {

	auto it = std::find_if(this -> _options.begin(), this -> _options.end(), [&option](const UCI::OPTION& o) { return option._id == o._id; });
	return it == this -> _options.end() ? -1 : std::distance(this -> _options.begin(), it);
}

const UCI::CATEGORY& UCI::SECTION::get_category() const {

	auto it = std::find_if(this -> _package -> _categories.begin(), this -> _package -> _categories.end(), [this](const UCI::CATEGORY& t) { return this -> _category_id == t._id; });

	if ( it == this -> _package -> _categories.end())
		throw std::runtime_error("get_category failed to find category with id " + std::to_string(this -> _category_id));

	return *it;
}

UCI::CATEGORY& UCI::SECTION::get_category() {

	auto it = std::find_if(this -> _package -> _categories.begin(), this -> _package -> _categories.end(), [this](const UCI::CATEGORY& t) { return this -> _category_id == t._id; });

	if ( it == this -> _package -> _categories.end())
		throw std::runtime_error("get_category failed to find category with id " + std::to_string(this -> _category_id));

	return *it;
}

long UCI::SECTION::next_id(bool push) {

	if ( push )
		return next_section_id++;
	else
		return next_section_id;
}

void UCI::SECTION::push_next_id() {
	next_section_id++;
}
