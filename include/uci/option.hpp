#pragma once
#include <string>
#include <vector>
#include <variant>
#include <ostream>

#include "uci/type.hpp"

namespace UCI {

	class PACKAGE;
	class SECTION;

	class OPTION {

		friend class PACKAGE;
		friend class SECTION;

		public:

			using iterator = std::vector<OPTION>::iterator;
			using const_iterator = std::vector<OPTION>::const_iterator;

			bool operator ==(const UCI::TYPE& type) const;
			bool operator !=(const UCI::TYPE& type) const;

			bool operator ==(const std::string& value);
			bool operator ==(const char* value);
			bool operator ==(const long long& value);
			bool operator ==(const int& value);
			bool operator ==(const long double& value);
			bool operator ==(const double& value);
			bool operator ==(const float& value);
			bool operator ==(const bool& value);
			bool operator ==(const std::vector<OPTION>& value);
			bool operator ==(const OPTION& other);

			OPTION& operator =(const std::string& value);
			OPTION& operator =(const char* value);
			OPTION& operator =(const long long& value);
			OPTION& operator =(const int& value);
			OPTION& operator =(const long double& value);
			OPTION& operator =(const double& value);
			OPTION& operator =(const float& value);
			OPTION& operator =(const bool& value);
			OPTION& operator =(const std::vector<OPTION>& value);
			OPTION& operator =(const OPTION& other);

			operator std::string() const;
			operator long long() const;
			operator int() const;
			operator long double() const;
			operator double() const;
			operator float() const;
			operator bool() const;

			UCI::OPTION& operator [](const size_t& index);
			const UCI::OPTION& operator [](const size_t& index) const;

			iterator begin();
			iterator end();
			const_iterator begin() const;
			const_iterator end() const;
			const_iterator cbegin() const;
			const_iterator cend() const;

			size_t size() const;
			bool empty() const;

			std::string name() const;
			size_t index() const;

			UCI::TYPE type() const;

			bool is_convertible(const UCI::TYPE& to) const;
			bool convertible_to(const UCI::TYPE& type) const;

			std::string to_string() const;
			long double to_float() const;
			long long to_number() const;
			bool to_bool() const;

			UCI::OPTION& add(const UCI::OPTION& option);
			void remove(const UCI::OPTION& option);
			void remove();

			OPTION(const std::string& value);
			OPTION(const char* value);
			OPTION(const long double& value);
			OPTION(const double& value);
			OPTION(const float& value);
			OPTION(const long long& value);
			OPTION(const int& value);
			OPTION(const bool& value);
			OPTION(const std::vector<OPTION>& value);

		private:

			PACKAGE* _package = nullptr;
			long _category_id = -1;
			long _section_id = -1;
			long _parent_id = -1;
			long _id = -1;

			std::string _name = "";
			std::variant<std::string, long long, long double, bool, std::vector<OPTION>, std::nullptr_t> _value = nullptr;

			void reset();
			void set_ids(long category_id, long section_id, long id);
			void set_ids(long category_id, long section_id, long parent_id, long id);

			const OPTION& get_parent() const;
			OPTION& get_parent();

			OPTION();
			OPTION(PACKAGE* package, const std::string& name, const std::nullptr_t& n);
			OPTION(PACKAGE* package, const std::string& name, const std::string& value);
			OPTION(PACKAGE* package, const std::string& name, const long long& value);
			OPTION(PACKAGE* package, const std::string& name, const long double& value);
			OPTION(PACKAGE* package, const std::string& name, const bool& value);
			OPTION(PACKAGE* package, const std::string& name, const std::vector<OPTION>& array);
	};

};

std::ostream& operator <<(std::ostream& os, const UCI::OPTION& option);
