#pragma once
#include <string>
#include <vector>
#include <optional>
#include <ostream>

namespace UCI {

	class CATEGORY;
	class SECTION;
	class OPTION;

	class PACKAGE {

		friend class CATEGORY;
		friend class SECTION;
		friend class OPTION;

		public:

			using iterator = std::vector<UCI::CATEGORY>::iterator;
			using const_iterator = std::vector<UCI::CATEGORY>::const_iterator;

			UCI::CATEGORY& operator [](const std::string& category);
			UCI::CATEGORY& operator [](size_t index);
			const UCI::CATEGORY& operator [](const std::string& category) const;
			const UCI::CATEGORY& operator [](size_t index) const;

			iterator begin();
			iterator end();
			const_iterator begin() const;
			const_iterator end() const;
			const_iterator cbegin() const;
			const_iterator cend() const;

			size_t size() const;
			bool empty() const;
			bool contains(const std::string& category) const;

			std::string package() const;
			std::string filename() const;
			void clean_up();
			void set_filename(const std::string& package);
			std::string dump() const;
			void save();
			void save() const;

			UCI::SECTION& add(const std::string& category, const std::optional<std::string>& section);
			UCI::SECTION& add(const std::string& category);

			PACKAGE(const std::string& package);
			PACKAGE();

		private:
			std::string _package;
			std::vector<UCI::CATEGORY> _categories;

			size_t index_of(const UCI::CATEGORY& category) const;
			const UCI::CATEGORY& get_category(const long& category_id) const;
			UCI::CATEGORY& get_category(const long& category_id);

			void remove(const long& category_id, const long& section_id);
			void parse(const std::string& blob);

			static long new_category_id();
			static long get_category_id();
			static long new_section_id();
			static long get_section_id();
			static long new_option_id();
			static long get_option_id();
	};

}

std::ostream& operator <<(std::ostream& os, const UCI::PACKAGE& package);
