#include <algorithm>
#include <stdexcept>
#include "uci/util.hpp"
#include "uci.hpp"

static long next_type_id = 0;

UCI::SECTION& UCI::TYPE::operator [](const std::string& section) {

	std::string name = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(section))));

	if ( auto it = std::find_if(this -> _sections.begin(), this -> _sections.end(), [&name](const UCI::SECTION& s) {
			return s._name == name;
		}); it != this -> _sections.end())
		return *it;

	throw std::runtime_error("section " + name + " is out of bounds, section not found");
}

UCI::SECTION& UCI::TYPE::operator [](size_t index) {

	if ( this -> _sections.empty())
		throw std::runtime_error("section at index " + std::to_string(index) + " is out of bounds, type " + this -> _name + " is empty");

	if ( index >= this -> _sections.size())
		throw std::runtime_error("section at index " + std::to_string(index) + " is out of bounds, last available index is " + std::to_string(this -> _sections.size() - 1 ));

	auto it = this -> _sections.begin();
	if ( index > 0 )
		std::advance(it, index);

	return *it;
}

const UCI::SECTION& UCI::TYPE::operator [](const std::string& section) const {

	std::string name = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(section))));

	if ( auto it = std::find_if(this -> _sections.begin(), this -> _sections.end(), [&name](const UCI::SECTION& s) {
			return s._name == name;
		}); it != this -> _sections.end())
		return *it;

	throw std::runtime_error("section " + name + " is out of bounds, section not found");
}

const UCI::SECTION& UCI::TYPE::operator [](size_t index) const {

	if ( this -> _sections.empty())
		throw std::runtime_error("section at index " + std::to_string(index) + " is out of bounds, type " + this -> _name + " is empty");

	if ( index >= this -> _sections.size())
		throw std::runtime_error("section at index " + std::to_string(index) + " is out of bounds, last available index is " + std::to_string(this -> _sections.size() - 1 ));

	auto it = this -> _sections.begin();
	if ( index > 0 )
		std::advance(it, index);

	return *it;
}

UCI::TYPE::iterator UCI::TYPE::begin() {
	return this -> _sections.begin();
}

UCI::TYPE::iterator UCI::TYPE::end() {
	return this -> _sections.end();
}

UCI::TYPE::const_iterator UCI::TYPE::begin() const {
	return this -> _sections.cbegin();
}

UCI::TYPE::const_iterator UCI::TYPE::end() const {
	return this -> _sections.cend();
}

UCI::TYPE::const_iterator UCI::TYPE::cbegin() const {
	return this -> _sections.cbegin();
}

UCI::TYPE::const_iterator UCI::TYPE::cend() const {
	return this -> _sections.cend();
}

std::string UCI::TYPE::name() const {
	return this -> _name;
}

size_t UCI::TYPE::index() const {
	return this -> _package -> index_of(*this);
}

size_t UCI::TYPE::size() const {
	return this -> _sections.size();
}

bool UCI::TYPE::empty() const {
	return this -> _sections.empty();
}

bool UCI::TYPE::contains(const std::string& section) const {

	std::string name = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(section))));

	return std::find_if(this -> _sections.begin(), this -> _sections.end(), [&name](const UCI::SECTION& s) {
			return s._name != std::nullopt && name == *s._name;
		}) != this -> _sections.end();

}

UCI::SECTION& UCI::TYPE::add(const std::optional<std::string>& section) {

	if ( section != std::nullopt ) {

		std::string name = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(*section))));

		if ( std::find_if(this -> _sections.begin(), this -> _sections.end(), [&name](const UCI::SECTION& s) {
				return s._name != std::nullopt && name == *s._name;
			}) != this -> _sections.end())
			throw std::runtime_error("duplicate section " + name);

		this -> _sections.push_back(SECTION(this -> _package, this -> _id, UCI::SECTION::next_id(true), name));
		return this -> _sections.back();
	}

	this -> _sections.push_back(SECTION(this -> _package, this -> _id, UCI::SECTION::next_id(true)));
	return this -> _sections.back();
}

size_t UCI::TYPE::index_of(const UCI::SECTION& section) const {

	auto it = std::find_if(this -> _sections.begin(), this -> _sections.end(), [&section](const UCI::SECTION& s) { return section._id == s._id; });
	return it == this -> _sections.end() ? -1 : std::distance(this -> _sections.begin(), it);
}

long UCI::TYPE::next_id(bool push) {

	if ( push )
		return next_type_id++;
	else
		return next_type_id;
}

void UCI::TYPE::push_next_id() {
	next_type_id++;
}
