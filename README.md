[![License:MIT](https://img.shields.io/badge/License-MIT-blue?style=plastic)](LICENSE)
[![C++ CI build](../../actions/workflows/build.yml/badge.svg)](../../actions/workflows/build.yml)

### uci_cpp
Small C++ library for Openwrt's ici config files

## <sub>usage</sub>
Sorry, no detailed instructions. Just something simple to get you started..

include uci.hpp and setup your uci package; you can give it a filename as argument,
if it is a path (somewhere in string is /) it will use file in that path; if it is
just a string(word), it assumes that it is /etc/config/FILENAME.

If you want to create new, you can ignore filename and later set it with set_filename();
and then use save() to write it.

Parsing content can be used by subscripts and iterators, also contains is available.
Structure of package goes like this:

`pkg[type][section][option/list][[ optional list option]]`

For example..
```
config interface eth0
	option ipaddress 192.168.1.1
	list dns 1.1.1.1
	list dns 8.8.8.8

config interface
	option something 1
```

to access eth0... `pkg["interface"]["eth0"]` and option.. `["ipadrress"]` or `["dns"][0]` to get 1.1.1.1
while using lists, you can use `size()` to check amount of elements in container. Or `empty()` to verify if it
is or is not empty..

To access anonymous section.. `pkg["interface"][1]` ( index 0 is "eth0" )
you can also iterate in every struct, except option (or sub-option) - which will throw.

Use `add()` (available in various classes, check headers) to create new sections or options/lists.
You can also use `add()` to add to list.. `pkg["interface"]["eth0"]["dns"].add("2.2.2.2");` beware, nested lists will throw.

Finally, if you make changes to your uci config, you can call `pkg.clean_up()` to remove any clutter; this means
that every type and section without options, will be removed.

You can use `dump()` to output contents similarly as uci client app does it.. All members also have index() available.
`pkg.filedata()` contains current config's data, `save()` writes that content to file.

You can remove things also, for example `pkg["interface"]["eth0"].remove();` - this is also available for options.

### <sub>room for improvement?</sub>
Yes, always - but this library tolerates comments, but unfortunately when you save your config, it "cleans it up", so for example,
all comments are lost. I mostly made this for reading uci config's, so that might be something to look in the future..

### <sub>about lists</sub>
You might need to cast when adding a list option, `(std::vector<UCI::OPTION>){ "my option text" }`. While testing, without
casting it usually understood this as just invidual option string, not as list.

### <sub>option types</sub>
available types are string, int, float, bool and array. Bools are always parsed incorrectly, as they are either int (if expressed as 1 or 0)
or string ( expressed as 'on', 'true', 'enabled', etc.. ).

You can get type of option with `type()`. Types also have comparison features available.

### <sub>option casting</sub>
you can check if option can be casted to certain type. Type definitions:
 - UCI::TYPES::STRING
 - UCI::TYPES::INT
 - UCI::TYPES::FLOAT
 - UCI::TYPES::BOOL

and also UCI::TYPES::ARRAY, but for obvious reasons, you cannot cast to that type.

casting is done with option's `convertible_to(type)` and to do actual casting, you can use
 - to_string()
 - to_float()
 - to_int()
 - to_bool()

anything can be casted to string (even array). If casting to float, int or bool fails, it throws - but this
is something that you can avoid by using `convertible_to(UCI::TYPES::*type*)`

### <sub>what next?</sub>
ostream support would be nice

### <sub>importing</sub>
Yes, clone as submodule to uci in root of your project.
Then in your makefile, include uci/Makefile.inc and link against `$(UCI_OBJS)`

### <sub>sample?</sub>
some sample code is in main.cpp

### <sub>license</sub>
MIT
