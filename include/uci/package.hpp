#pragma once
#include <string>
#include <vector>
#include <optional>

namespace UCI {

	class TYPE;
	class SECTION;
	class OPTION;

	class PACKAGE {

		friend class TYPE;
		friend class SECTION;
		friend class OPTION;

		public:

			using iterator = std::vector<UCI::TYPE>::iterator;
			using const_iterator = std::vector<UCI::TYPE>::const_iterator;

			UCI::TYPE& operator [](const std::string& type);
			UCI::TYPE& operator [](size_t index);
			const UCI::TYPE& operator [](const std::string& type) const;
			const UCI::TYPE& operator [](size_t index) const;

			iterator begin();
			iterator end();
			const_iterator begin() const;
			const_iterator end() const;
			const_iterator cbegin() const;
			const_iterator cend() const;

			size_t size() const;
			bool empty() const;
			bool contains(const std::string& type) const;

			std::string package() const;
			std::string filename() const;
			void clean_up();
			void set_filename(const std::string& package);
			std::string filedata() const;
			void save();
			void save() const;
			void dump() const;

			UCI::SECTION& add(const std::string& type, const std::optional<std::string>& section);
			UCI::SECTION& add(const std::string& type);

			PACKAGE(const std::string& package);

		private:
			std::string _package;
			std::vector<UCI::TYPE> _types;

			size_t index_of(const UCI::TYPE& type) const;
			UCI::TYPE& get_type(const long& type_id);

			void remove(const long& type_id, const long& section_id);
			void parse(const std::string& blob);

			PACKAGE();
	};

}
