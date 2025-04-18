#pragma once
#include <string>
#include <vector>
#include <optional>

namespace UCI {

	class PACKAGE;
	class SECTION;
	class OPTION;

	class CATEGORY {

		friend class PACKAGE;
		friend class SECTION;
		friend class OPTION;

		public:

			using iterator = std::vector<UCI::SECTION>::iterator;
			using const_iterator = std::vector<UCI::SECTION>::const_iterator;

			UCI::SECTION& operator [](const std::string& section);
			UCI::SECTION& operator [](size_t index);
			const UCI::SECTION& operator [](const std::string& section) const;
			const UCI::SECTION& operator [](size_t index) const;

			iterator begin();
			iterator end();
			const_iterator begin() const;
			const_iterator end() const;
			const_iterator cbegin() const;
			const_iterator cend() const;

			std::string name() const;
			size_t index() const;
			size_t size() const;
			bool empty() const;
			bool contains(const std::string& section) const;

			UCI::SECTION& add(const std::optional<std::string>& section);

		private:

			PACKAGE* _package = nullptr;
			long _id = -1;

			std::string _name;
			std::vector<UCI::SECTION> _sections = {};

			size_t index_of(const UCI::SECTION& section) const;

			CATEGORY(PACKAGE* package, long id, const std::string& name) : _package(package), _id(id), _name(name) {}

			static long next_id(bool push = false);
			static void push_next_id();
	};

};
