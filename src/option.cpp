#include <algorithm>
#include <stdexcept>
#include "uci/util.hpp"
#include "uci.hpp"

bool UCI::OPTION::operator ==(const UCI::TYPE& type) const {
	return this -> type() == type;
}

bool UCI::OPTION::operator !=(const UCI::TYPE& type) const {
	return this -> type() != type;
}

bool UCI::OPTION::operator ==(const std::string& value) {

	if ( this -> type() == UCI::TYPE::ARRAY )
		return false;

	return this -> to_string() == value;
}

bool UCI::OPTION::operator ==(const char* value) {

	return this -> operator ==(std::string(value));
}

bool UCI::OPTION::operator ==(const long long& value) {

	if ( this -> type() == UCI::TYPE::ARRAY )
		return false;

	try {
		return this -> to_number() == value;
	} catch ( ... ) {
		return false;
	}
}

bool UCI::OPTION::operator ==(const int& value) {

	if ( this -> type() == UCI::TYPE::ARRAY )
		return false;

	try {
		return this -> to_number() == (long long)value;
	} catch ( ... ) {
		return false;
	}
}

bool UCI::OPTION::operator ==(const long double& value) {

	if ( this -> type() == UCI::TYPE::ARRAY )
		return false;

	try {
		return this -> to_float() == value;
	} catch ( ... ) {
		return false;
	}
}

bool UCI::OPTION::operator ==(const double& value) {

	if ( this -> type() == UCI::TYPE::ARRAY )
		return false;

	try {
		return this -> to_float() == (long double)value;
	} catch ( ... ) {
		return false;
	}
}

bool UCI::OPTION::operator ==(const float& value) {

	if ( this -> type() == UCI::TYPE::ARRAY )
		return false;

	try {
		return this -> to_float() == (long double)value;
	} catch ( ... ) {
		return false;
	}
}

bool UCI::OPTION::operator ==(const bool& value) {

	if ( this -> type() == UCI::TYPE::ARRAY )
		return false;

	try {
		return this -> to_bool() ==  value;
	} catch ( ... ) {
		return false;
	}
}

bool UCI::OPTION::operator ==(const std::vector<UCI::OPTION>& value) {

	if ( this -> type() == UCI::TYPE::ARRAY || this -> size() != value.size())
		return false;

	size_t index = 0;

	for ( auto it = this -> begin(); it != this -> end(); it++ ) {

		if ( it -> to_string() != value.at(index).to_string())
			return false;

		index++;
	}

	return true;
}

bool UCI::OPTION::operator ==(const OPTION& other) {

	return this -> to_string() == other.to_string();
}


UCI::OPTION& UCI::OPTION::operator =(const std::string& value) {
	this -> _value.emplace<std::string>(std::forward<decltype(value)>(value));
	return *this;
}

UCI::OPTION& UCI::OPTION::operator =(const char* value) {
	std::string s(value);
	this -> _value.emplace<std::string>(std::forward<decltype(s)>(s));
	return *this;
}

UCI::OPTION& UCI::OPTION::operator =(const long long& value) {
	this -> _value.emplace<long long>(std::forward<decltype(value)>(value));
	return *this;
}

UCI::OPTION& UCI::OPTION::operator =(const int& value) {
	long long ll = (long long)value;
	this -> _value.emplace<long long>(std::forward<decltype(ll)>(ll));
	return *this;
}

UCI::OPTION& UCI::OPTION::operator =(const long double& value) {
	this -> _value.emplace<long double>(std::forward<decltype(value)>(value));
	return *this;
}

UCI::OPTION& UCI::OPTION::operator =(const double& value) {
	long double ld = (long double)value;
	this -> _value.emplace<long double>(std::forward<decltype(ld)>(ld));
	return *this;
}

UCI::OPTION& UCI::OPTION::operator =(const float& value) {
	long double ld = (long double)value;
	this -> _value.emplace<long double>(std::forward<decltype(ld)>(ld));
	return *this;
}

UCI::OPTION& UCI::OPTION::operator =(const bool& value) {
	this -> _value.emplace<bool>(std::forward<decltype(value)>(value));
	return *this;
}

UCI::OPTION& UCI::OPTION::operator =(const std::vector<OPTION>& value) {

	if ( this -> _parent_id != -1 )
		throw std::runtime_error("invalid assignment, nested lists are not allowed");

	this -> _value.emplace<std::vector<UCI::OPTION>>(std::forward<decltype(value)>(value));

	std::vector<UCI::OPTION>& vec = std::get<std::vector<UCI::OPTION>>(this -> _value);
	for ( auto it = vec.begin(); it != vec.end(); it++ ) {

		it -> _name = this -> _name;
		it -> _id = UCI::PACKAGE::new_option_id();
		it -> _parent_id = this -> _id;
		it -> _section_id = this -> _section_id;
		it -> _category_id = this -> _category_id;
		it -> _package = this -> _package;
	}

	return *this;
}

UCI::OPTION& UCI::OPTION::operator =(const UCI::OPTION& other) {

	if ( this -> _id == -1 )
		this -> _id = UCI::PACKAGE::new_option_id();

	if ( other.type() == UCI::STRING ) {
		std::string s = std::get<std::string>(other._value);
		this -> _value.emplace<std::string>(std::forward<decltype(s)>(s));
	} else if ( other.type() == UCI::FLOAT ) {
		long double ld = std::get<long double>(other._value);
		this -> _value.emplace<long double>(std::forward<decltype(ld)>(ld));
	} else if ( other.type() == UCI::INT ) {
		long long ll = std::get<long long>(other._value);
		this -> _value.emplace<long long>(std::forward<decltype(ll)>(ll));
	} else if ( other.type() == UCI::BOOL ) {
		bool b = std::get<bool>(other._value);
		this -> _value.emplace<bool>(std::forward<decltype(b)>(b));
	} else if ( other.type() == UCI::ARRAY ) {

		if ( this -> _parent_id != -1 )
			throw std::runtime_error("invalid assignment, nested lists are not allowed");

		std::vector<UCI::OPTION> arr = std::get<std::vector<UCI::OPTION>>(other._value);
		this -> _value.emplace<std::vector<UCI::OPTION>>(std::forward<decltype(arr)>(arr));

		std::vector<UCI::OPTION>& vec = std::get<std::vector<UCI::OPTION>>(this -> _value);
		for ( auto it = vec.begin(); it != vec.end(); it++ ) {

			it -> _name = this -> _name;
			it -> _id = UCI::PACKAGE::new_option_id();
			it -> _parent_id = this -> _id;
			it -> _section_id = this -> _section_id;
			it -> _category_id = this -> _category_id;
			it -> _package = this -> _package;
		}

	} else {

		std::nullptr_t n = nullptr;
		this -> _value.emplace<std::nullptr_t>(std::forward<decltype(n)>(n));
	}

	return *this;
}

UCI::OPTION::operator std::string() const {
	return this -> to_string();
}

UCI::OPTION::operator long long() const {
	return this -> to_number();
}

UCI::OPTION::operator int() const {
	return (int)this -> to_number();
}

UCI::OPTION::operator long double() const {
	return this -> to_float();
}

UCI::OPTION::operator double() const {
	return (double)this -> to_float();
}

UCI::OPTION::operator float() const {
	return (float)this -> to_float();
}

UCI::OPTION::operator bool() const {
	return (bool)this -> to_bool();
}

UCI::OPTION& UCI::OPTION::operator [](const size_t& index) {

	if ( this -> type() != UCI::ARRAY )
		throw std::runtime_error("index subscript is available only for lists");
	else if ( this -> empty())
		throw std::runtime_error("out of bounds, option " + this -> _name + " does not have any options in list");
	else if ( index >= this -> size())
		throw std::runtime_error("out of bounds, option " + this -> _name + " last index is " + std::to_string(this -> size() - 1));

	auto it = std::get<std::vector<UCI::OPTION>>(this -> _value).begin();
	if ( index > 0 )
		std::advance(it, index);

	return *it;
}

const UCI::OPTION& UCI::OPTION::operator [](const size_t& index) const {

	if ( this -> type() != UCI::ARRAY )
		throw std::runtime_error("index subscript is available only for lists");
	else if ( this -> empty())
		throw std::runtime_error("out of bounds, option " + this -> _name + " does not have any options in list");
	else if ( index >= this -> size())
		throw std::runtime_error("out of bounds, option " + this -> _name + " last index is " + std::to_string(this -> size() - 1));

	auto it = std::get<std::vector<UCI::OPTION>>(this -> _value).begin();
	if ( index > 0 )
		std::advance(it, index);

	return *it;
}

UCI::OPTION::iterator UCI::OPTION::begin() {

	if ( this -> type() != UCI::ARRAY )
		throw std::runtime_error("cannot iterate option which is not a list");

	return std::get<std::vector<UCI::OPTION>>(this -> _value).begin();
}

UCI::OPTION::iterator UCI::OPTION::end() {

	if ( this -> type() != UCI::ARRAY )
		throw std::runtime_error("cannot iterate option which is not a list");

	return std::get<std::vector<UCI::OPTION>>(this -> _value).end();
}

UCI::OPTION::const_iterator UCI::OPTION::begin() const {

	if ( this -> type() != UCI::ARRAY )
		throw std::runtime_error("cannot iterate option which is not a list");

	return std::get<std::vector<UCI::OPTION>>(this -> _value).cbegin();
}

UCI::OPTION::const_iterator UCI::OPTION::end() const {

	if ( this -> type() != UCI::ARRAY )
		throw std::runtime_error("cannot iterate option which is not a list");

	return std::get<std::vector<UCI::OPTION>>(this -> _value).cend();
}

UCI::OPTION::const_iterator UCI::OPTION::cbegin() const {

	if ( this -> type() != UCI::ARRAY )
		throw std::runtime_error("cannot iterate option which is not a list");

	return std::get<std::vector<UCI::OPTION>>(this -> _value).cbegin();
}

UCI::OPTION::const_iterator UCI::OPTION::cend() const {

	if ( this -> type() != UCI::ARRAY )
		throw std::runtime_error("cannot iterate option which is not a list");

	return std::get<std::vector<UCI::OPTION>>(this -> _value).cend();
}

size_t UCI::OPTION::size() const {

	if ( this -> type() != UCI::ARRAY )
		return 0;

	return std::get<std::vector<UCI::OPTION>>(this -> _value).size();
}

bool UCI::OPTION::empty() const {

	if ( this -> type() != UCI::ARRAY )
		return true;

	return std::get<std::vector<UCI::OPTION>>(this -> _value).empty();
}

std::string UCI::OPTION::name() const {
	return this -> _name;
}

size_t UCI::OPTION::index() const {

	if ( this -> type() == UCI::TYPE::ARRAY && this -> _parent_id != -1 ) {

		long id = this -> _id;
		const std::vector<UCI::OPTION>& parent = std::get<std::vector<UCI::OPTION>>(this -> get_parent()._value);

		auto it = std::find_if(parent.begin(), parent.end(), [&id](const UCI::OPTION& o) { return id == o._id; });
		return it == parent.end() ? -1 : std::distance(parent.begin(), it);

	}

	try {
		return this -> _package -> get_category(this -> _category_id).get_section(this -> _section_id).index_of(*this);
	} catch ( const std::runtime_error& e ) {
		throw std::runtime_error(std::string(e.what()) + ", cannot get index of option " + this -> _name);
	}
}

UCI::TYPE UCI::OPTION::type() const {

	if ( std::holds_alternative<std::vector<UCI::OPTION>>(this -> _value))
		return UCI::TYPE::ARRAY;
	else if ( std::holds_alternative<std::string>(this -> _value))
		return UCI::TYPE::STRING;
	else if ( std::holds_alternative<bool>(this -> _value))
		return UCI::TYPE::BOOL;
	else if ( std::holds_alternative<long long>(this -> _value))
		return UCI::TYPE::INT;
	else if ( std::holds_alternative<long double>(this -> _value))
		return UCI::TYPE::FLOAT;
	else return UCI::TYPE::STRING;
}

bool UCI::OPTION::is_convertible(const UCI::TYPE& to) const {

	if ( to == UCI::TYPE::STRING )
		return true;
	else if ( to == UCI::TYPE::FLOAT ) {

		if ( this -> type() == UCI::TYPE::FLOAT || this -> type() == UCI::TYPE::INT || this -> type() == UCI::TYPE::BOOL )
			return true;
		else if ( this -> type() == UCI::TYPE::STRING ) {

			std::string value = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(std::get<std::string>(this -> _value)))));
			try {
				std::stold(value);
			} catch ( ... ) {
				return false;
			}

			return true;
		}

		return false;

	} else if ( to == UCI::TYPE::INT ) {

		if ( this -> type() == UCI::TYPE::FLOAT || this -> type() == UCI::TYPE::INT || this -> type() == UCI::TYPE::BOOL )
			return true;
		else if ( this -> type() == UCI::TYPE::STRING ) {

			std::string value = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(std::get<std::string>(this -> _value)))));
			try {
				std::stoll(value);
			} catch ( ... ) {
				return false;
			}

			return true;
		}

		return false;

	} else if ( to == UCI::TYPE::BOOL ) {

		if ( this -> type() == UCI::TYPE::FLOAT || this -> type() == UCI::TYPE::INT || this -> type() == UCI::TYPE::BOOL )
			return true;
		else if ( this -> type() == UCI::TYPE::STRING ) {

			std::string value = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(std::get<std::string>(this -> _value)))));

			if ( value == "0" || value == "no" || value == "off" || value == "false" || value == "disabled" ||
				value == "1" || value == "yes" || value == "on" || value == "true" || value == "enabled" )
				return true;
		}

		return false;
	}

	return false;
}

bool UCI::OPTION::convertible_to(const UCI::TYPE& type) const {

	return this -> is_convertible(type);
}

std::string UCI::OPTION::to_string() const {

	if ( this -> type() == UCI::TYPE::STRING )
		return std::get<std::string>(this -> _value);
	else if ( this -> type() == UCI::TYPE::FLOAT ) {

		std::string s = std::to_string(std::get<long double>(this -> _value));

		if ( s.find_first_of('.') != std::string::npos ) {

			while ( s.back() == '0' ) // trim trailing zeros
				s.pop_back();

			if ( s.back() == '.' )
				s.pop_back();

			if ( s.empty())
				s = "0";
		}

		return s;

	} else if ( this -> type() == UCI::TYPE::INT )
		return std::to_string(std::get<long long>(this -> _value));
	else if ( this -> type() == UCI::TYPE::BOOL )
		return std::get<bool>(this -> _value) == true ? "true" : "false";
	else if ( this -> type() == UCI::TYPE::ARRAY ) {

		std::string s;

		for ( auto it = this -> cbegin(); it != this -> cend(); it++ )
			s += ( !s.empty() ? ", " : "" ) + it -> to_string();

		return "[" + s + "]";
	}

	return "null";
}

long double UCI::OPTION::to_float() const {

	if ( this -> type() == UCI::TYPE::FLOAT )
		return std::get<long double>(this -> _value);
	else if ( this -> type() == UCI::TYPE::INT )
		return (long double)std::get<long long>(this -> _value);
	else if ( this -> type() == UCI::TYPE::BOOL )
		return (long double)(std::get<bool>(this -> _value) ? 1 : 0 );
	else if ( this -> type() == UCI::TYPE::STRING ) {

		long double ld = -1;
		std::string value = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(std::get<std::string>(this -> _value)))));

		try {
			ld = std::stold(value);
		} catch ( const std::invalid_argument& e ) {
			throw std::runtime_error("string '" + value + "' is not convertible to float value, value is not a number");
		} catch ( const std::out_of_range& e ) {
			throw std::runtime_error("string '" + value + "' is not convertible to float value, value is out of range");
		}

		return ld;

	} else if ( this -> type() == UCI::TYPE::ARRAY )
		throw std::runtime_error("array is not convertible to float value");

	throw std::runtime_error("option " + this -> _name + " is not convertible to float value");
}

long long UCI::OPTION::to_number() const {

	if ( this -> type() == UCI::TYPE::FLOAT )
		return (long long)std::get<long double>(this -> _value);
	else if ( this -> type() == UCI::TYPE::INT )
		return std::get<long long>(this -> _value);
	else if ( this -> type() == UCI::TYPE::BOOL )
		return (long long)(std::get<bool>(this -> _value) ? 1 : 0);
	else if ( this -> type() == UCI::TYPE::STRING ) {

		long long ll = -1;
		std::string value = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(std::get<std::string>(this -> _value)))));

		try {
			ll = std::stoll(value);
		} catch ( const std::invalid_argument& e ) {
			throw std::runtime_error("string '" + value + "' is not convertible to number value, value is not a number");
		} catch ( const std::out_of_range& e ) {
			throw std::runtime_error("string '" + value + "' is not convertible to number value, value is out of range");
		}

		return ll;

	} else if ( this -> type() == UCI::TYPE::ARRAY )
		throw std::runtime_error("array is not convertible to number value");

	throw std::runtime_error("option " + this -> _name + " is not convertible to number value");
}

bool UCI::OPTION::to_bool() const {

	if ( this -> type() == UCI::TYPE::FLOAT )
		return ((long long)std::get<long double>(this -> _value)) != 0;
	else if ( this -> type() == UCI::TYPE::INT )
		return std::get<long long>(this -> _value) != 0;
	else if ( this -> type() == UCI::TYPE::BOOL )
		return std::get<bool>(this -> _value);
	if ( this -> type() == UCI::TYPE::STRING ) {

		std::string value = UCI::STR::to_lower(UCI::STR::trim(UCI::STR::unquoted(UCI::STR::trim(std::get<std::string>(this -> _value)))));

		if ( value.empty())
			throw std::runtime_error("empty string is not convertible to boolean value");

		if ( value == "0" || value == "no" || value == "off" || value == "false" || value == "disabled" )
			return false;
		else if ( value == "1" || value == "yes" || value == "on" || value == "true" || value == "enabled" )
			return true;

		throw std::runtime_error("string value '" + value + "' is not convertible to boolean value");

	} else if ( this -> type() == UCI::TYPE::ARRAY )
		throw std::runtime_error("array is not convertible to boolean value");

	throw std::runtime_error("option " + this -> _name + " is not convertible to boolean value");
}

UCI::OPTION& UCI::OPTION::add(const UCI::OPTION& option) {

	if ( this -> type() != UCI::ARRAY )
		throw std::runtime_error("cannot add, adding is not available for option which is not a list");

	if ( option.type() == UCI::ARRAY )
		throw std::runtime_error("invalid add, nested lists are not allowed");

	OPTION opt = option;
	opt._package = this -> _package;
	opt._category_id = this -> _category_id;
	opt._section_id = this -> _section_id;
	opt._parent_id = this -> _id;
	opt._name = this -> _name;

	std::get<std::vector<UCI::OPTION>>(this -> _value).push_back(opt);
	return std::get<std::vector<UCI::OPTION>>(this -> _value).back();
}

void UCI::OPTION::remove() {

	if ( this -> _parent_id != -1 && this -> get_parent().type() == UCI::TYPE::ARRAY )
		this -> get_parent().remove(*this);
	else if ( this -> _section_id != -1 ) {

		try {
			this -> _package -> get_category(this -> _category_id).get_section(this -> _section_id).remove(*this);
		} catch ( const std::runtime_error& e ) {
			throw std::runtime_error(std::string(e.what()) + ", cannot remove option " + this -> _name);
		}
	}
}

void UCI::OPTION::remove(const UCI::OPTION& option) {

	if ( this -> type() != UCI::ARRAY )
		throw std::runtime_error("option::remove(::option) is available only for lists");

	std::vector<UCI::OPTION>& vec = std::get<std::vector<UCI::OPTION>>(this -> _value);
	vec.erase(std::remove_if(vec.begin(), vec.end(), [&option](const UCI::OPTION& o) {
			return option.type() == o.type() && option.to_string() == o.to_string();
		}), vec.end());

	if ( this -> _package != nullptr )
		this -> _package -> clean_up();
}

void UCI::OPTION::reset() {

	this -> _package = nullptr;
	this -> _category_id = -1;
	this -> _section_id = -1;
	this -> _parent_id = -1;
	this -> _id = -1;
}

UCI::OPTION::OPTION(const std::string& value) {

	this -> reset();
	this -> _value.emplace<std::string>(std::forward<decltype(value)>(value));
}

UCI::OPTION::OPTION(const char* value) {

	std::string s(value);

	this -> reset();
	this -> _value.emplace<std::string>(std::forward<decltype(s)>(s));
}

UCI::OPTION::OPTION(const long double& value) {

	this -> reset();
	this -> _value.emplace<long double>(std::forward<decltype(value)>(value));
}

UCI::OPTION::OPTION(const double& value) {

	long double ld = (long double)value;

	this -> reset();
	this -> _value.emplace<long double>(std::forward<decltype(ld)>(ld));
}

UCI::OPTION::OPTION(const float& value) {

	long double ld = (long double)value;

	this -> reset();
	this -> _value.emplace<long double>(std::forward<decltype(ld)>(ld));
}

UCI::OPTION::OPTION(const long long& value) {

	this -> reset();
	this -> _value.emplace<long long>(std::forward<decltype(value)>(value));
}

UCI::OPTION::OPTION(const int& value) {

	long long ll = (long long)value;

	this -> reset();
	this -> _value.emplace<long long>(std::forward<decltype(ll)>(ll));
}

UCI::OPTION::OPTION(const bool& value) {

	this -> reset();
	this -> _value.emplace<bool>(std::forward<decltype(value)>(value));
}

UCI::OPTION::OPTION(const std::vector<UCI::OPTION>& value) {

	this -> reset();
	this -> _value.emplace<std::vector<UCI::OPTION>>(std::forward<decltype(value)>(value));

	std::vector<UCI::OPTION>& vec = std::get<std::vector<UCI::OPTION>>(this -> _value);
	for ( auto it = vec.begin(); it != vec.end(); it++ )
		it -> reset();

}

const UCI::OPTION& UCI::OPTION::get_parent() const {

	try {
		UCI::SECTION& section = this -> _package -> get_category(this -> _category_id).get_section(this -> _section_id);

		if ( auto it = std::find_if(section._options.begin(), section._options.end(), [this](const UCI::OPTION& o) { return this -> _parent_id == o._id; });
			it != section._options.end())
			return *it;
	} catch ( const std::runtime_error& e ) {
		throw e;
	}

	throw std::runtime_error("get parent failed to find option with id " + std::to_string(this -> _parent_id));
}

UCI::OPTION& UCI::OPTION::get_parent() {

	try {
		UCI::SECTION& section = this -> _package -> get_category(this -> _category_id).get_section(this -> _section_id);

		if ( auto it = std::find_if(section._options.begin(), section._options.end(), [this](const UCI::OPTION& o) { return this -> _parent_id == o._id; });
			it != section._options.end())
			return *it;
	} catch ( const std::runtime_error& e ) {
		throw e;
	}

	throw std::runtime_error("get parent failed to find option with id " + std::to_string(this -> _parent_id));
}

void UCI::OPTION::set_ids(long category_id, long section_id, long id) {

	this -> _category_id = category_id;
	this -> _section_id = section_id;
	this -> _parent_id = -1;
	this -> _id = id;
}

void UCI::OPTION::set_ids(long category_id, long section_id, long parent_id, long id) {

	this -> _category_id = category_id;
	this -> _section_id = section_id;
	this -> _parent_id = parent_id;
	this -> _id = id;
}

UCI::OPTION::OPTION() {

	std::nullptr_t n = nullptr;

	this -> reset();
	this -> _value.emplace<std::nullptr_t>(std::forward<decltype(n)>(n));
}

UCI::OPTION::OPTION(UCI::PACKAGE* package, const std::string& name, const std::nullptr_t& n) {

	this -> reset();
	this -> _package = package;
	this -> _name = name;

	this -> _value.emplace<std::nullptr_t>(std::forward<decltype(n)>(n));
}

UCI::OPTION::OPTION(UCI::PACKAGE* package, const std::string& name, const std::string& value) {

	this -> reset();
	this -> _package = package;
	this -> _name = name;

	this -> _value.emplace<std::string>(std::forward<decltype(value)>(value));
}

UCI::OPTION::OPTION(UCI::PACKAGE* package, const std::string& name, const long long& value) {

	this -> reset();
	this -> _package = package;
	this -> _name = name;

	this -> _value.emplace<long long>(std::forward<decltype(value)>(value));
}

UCI::OPTION::OPTION(UCI::PACKAGE* package, const std::string& name, const long double& value) {

	this -> reset();
	this -> _package = package;
	this -> _name = name;

	this -> _value.emplace<long double>(std::forward<decltype(value)>(value));
}

UCI::OPTION::OPTION(UCI::PACKAGE* package, const std::string& name, const bool& value) {

	this -> reset();
	this -> _package = package;
	this -> _name = name;

	this -> _value.emplace<bool>(std::forward<decltype(value)>(value));
}

UCI::OPTION::OPTION(UCI::PACKAGE* package, const std::string& name, const std::vector<UCI::OPTION>& array) {

	this -> reset();
	this -> _package = package;
	this -> _name = name;

	this -> _value.emplace<std::vector<OPTION>>(std::forward<decltype(array)>(array));

	std::vector<UCI::OPTION>& vec = std::get<std::vector<UCI::OPTION>>(this -> _value);
	for ( auto it = vec.begin(); it != vec.end(); it++ ) {

		if ( it -> type() == UCI::TYPE::ARRAY )
			throw std::runtime_error("invalid list, nested lists are not allowed");

		it -> reset();
		it -> _package = package;
		it -> _name = name;
	}
}

std::ostream& operator <<(std::ostream& os, const UCI::OPTION& option) {
	os << option.name() << "=" << option.to_string();
	return os;
}
