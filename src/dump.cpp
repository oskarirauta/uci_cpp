#include <iostream>
#include "uci.hpp"

void UCI::PACKAGE::dump() const {

	for ( auto type_it = this -> _types.begin(); type_it != this -> _types.end(); type_it++ ) {

		for ( auto section_it = type_it -> _sections.begin(); section_it != type_it -> _sections.end(); section_it++ ) {

			for ( auto option_it = section_it -> _options.begin(); option_it != section_it -> _options.end(); option_it++ ) {

				std::cout << type_it -> _name << ( section_it -> is_anonymous() ? "" : "." )
					<< section_it -> name() << "." << option_it -> _name << "=";

				if ( option_it -> type() == UCI::STRING )
					std::cout << "'" << std::get<std::string>(option_it -> _value) << "'";
				else if ( option_it -> type() == UCI::FLOAT )
					std::cout << std::get<long double>(option_it -> _value);
				else if ( option_it -> type() == UCI::INT )
					std::cout << std::get<long long>(option_it -> _value);
				else if ( option_it -> type() == UCI::BOOL && std::get<bool>(option_it -> _value) == true )
					std::cout << "true";
				else if ( option_it -> type() == UCI::BOOL && std::get<bool>(option_it -> _value) == false )
					std::cout << "false";
				else if ( option_it -> type() == UCI::ARRAY ) {

					std::string s;
					const std::vector<UCI::OPTION>& array = std::get<std::vector<UCI::OPTION>>(option_it -> _value);

					for ( auto value_it = array.begin(); value_it != array.end(); value_it++ ) {

						s += ( !s.empty() ? ", " : "" );

						if ( value_it -> type() == UCI::STRING )
							s += "'" + std::get<std::string>(value_it -> _value) + "'";
						else if ( value_it -> type() == UCI::FLOAT )
							s += std::to_string(std::get<long double>(value_it -> _value));
						else if ( value_it -> type() == UCI::INT )
							s += std::to_string(std::get<long long>(value_it -> _value));
						else if ( value_it -> type() == UCI::BOOL && std::get<bool>(value_it -> _value) == true )
							s += "true";
						else if ( value_it -> type()  == UCI::BOOL && std::get<bool>(value_it -> _value) == false )
							s += "false";
						else s += "null";
					}

					std::cout << "[" << s << "]";

				} else std::cout << "null";

				std::cout << std::endl;
			}
		}
	}

}
