#pragma once
#include <string>
#include <vector>
#include <optional>
#include <ostream>

namespace UCI {

	class PACKAGE;
	class CATEGORY;
	class OPTION;

	class SECTION {

		friend class PACKAGE;
		friend class CATEGORY;
		friend class OPTION;

		public:

			using iterator = std::vector<UCI::OPTION>::iterator;
			using const_iterator = std::vector<UCI::OPTION>::const_iterator;

			UCI::OPTION& operator [](const std::string& option);
			UCI::OPTION& operator [](size_t index);
			const UCI::OPTION& operator [](const std::string& option) const;
			const UCI::OPTION& operator [](size_t index) const;

			iterator begin();
			iterator end();
			const_iterator begin() const;
			const_iterator end() const;
			const_iterator cbegin() const;
			const_iterator cend() const;

			bool is_anonymous() const;
			std::string name() const;
			size_t index() const;
			unsigned long id() const;

			size_t size() const;
			bool empty() const;
			bool contains(const std::string& option) const;

			UCI::OPTION& add(const std::string& name, const UCI::OPTION& option);
			void remove(const UCI::OPTION& option);
			void remove();

		private:

			PACKAGE* _package = nullptr;
			long _category_id = -1;
			long _id = -1;

			std::optional<std::string> _name;
			std::vector<UCI::OPTION> _options = {};

			size_t index_of(const UCI::OPTION& option) const;

			SECTION(PACKAGE* package, long category_id, long id) :
				_package(package), _category_id(category_id), _id(id), _name(std::nullopt) {}
			SECTION(PACKAGE* package, long category_id, long id, const std::optional<std::string>& name) :
				_package(package), _category_id(category_id), _id(id), _name(name) {}

	};

}

std::ostream& operator <<(std::ostream& os, const UCI::SECTION& section);
