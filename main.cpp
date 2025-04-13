#include <iostream>
#include "uci.hpp"

int main(int argc, char** argv) {

	UCI::PACKAGE pkg("./uci_sample");
/*
	UCI::SECTION* section = nullptr;
	section = &pkg.add("interface", "eth0");
	section -> add("ip", UCI::OPTION("10.0.0.1"));
	//section -> add("dns", UCI::OPTION("8.8.8.8"));
	pkg["interface"]["eth0"].add("dns", "1.1.1.1");

	section = &pkg.add("interface", "eth1");
	section -> add("ip", UCI::OPTION(std::string("10.0.0.2")));
	section -> add("name", UCI::OPTION("hello"));

	section = &pkg.add("global", std::nullopt);
	section -> add("netmask", UCI::OPTION("255.255.255.0"));
	section -> add("prefix", UCI::OPTION(24));
	section -> add("number", 9);
	section -> add ("bool", true);
	section -> add("info", "test");

	// initializer list: (broken)
	// section -> add("array", (std::vector<UCI::OPTION>){ "hello", "world" });

	section -> add("array", (std::vector<UCI::OPTION>){ "hello", "world" });

	pkg["global"][0]["info"] = "test2";

	std::cout << "index: " << pkg["interface"]["eth1"].index() << std::endl;

	std::cout << "interface index: " << pkg["interface"].index() << std::endl;
	std::cout << "eth0 index: " << pkg["interface"]["eth0"].index() << std::endl;
	std::cout << "dns index: " << pkg["interface"]["eth0"]["dns"].index() << std::endl;

	pkg["interface"]["eth0"]["dns"].remove();

	pkg["global"][0]["array"][1].remove();
*/
//	pkg.dump();

//	std::cout << "\n---\n" << std::endl;

	pkg.set_filename("./uci_sample2");
	pkg.save();

/*
	UCI uci("./ucntr");
	uci.print();

	auto t = uci["infra"];
	auto x = uci[1];

	std::cout << "t: " << t.name() << " x: " << x.name() << std::endl;
	std::cout << *t[1].name() << std::endl;
*/
	return 0;
}
