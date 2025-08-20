#include "gargoyle.hpp"
#include "gargoyle_argument.hpp"

#include <complex>
#include <iostream>
#include <string>
#include <vector>

Gargoyle::Gargoyle(int argc, char *argv[]) {
	for (int i = 1; i < argc; i++) {
		this->raw_arguments.push_back(argv[i]);
	}
}

bool Gargoyle::add_argument(GargoyleArgument argument) {
	std::string id = argument.get_id();
	if (this->arguments.count(id) == 1) {
		std::cerr << "This argument is already registered!";
		return (false);
	}
	auto ret = this->arguments.insert({id, argument});

	return (ret.second);
}

std::vector<std::string> Gargoyle::parse_argument(std::string argument) {
	std::vector<std::string> argument_ids = std::vector<std::string>();
	// DOUBLE_DASH
	if (argument.substr(0, 2) == "--") {
		std::string id = argument.substr(2);
		if (this->arguments.count(id) != 0 && this->arguments.at(id).get_flag() == GargoyleArgumentFlag::DOUBLE_DASH) {
			argument_ids.push_back(id);
		}
	}
	// TODO How do I handle multi-character single dash arguments?
	// Is "-qwer" 4 arguments? 1?
	// For now I'll assume it's either 1 argument (check the whole string) and otherwise it's a collection of 1 character arguments
	else if (argument.substr(0, 1) == "-") {
		// First check for multi character id
		std::string id = argument.substr(1);
		if (this->arguments.count(id) != 0 && this->arguments.at(id).get_flag() == GargoyleArgumentFlag::DASH) {
			argument_ids.push_back(id);
			return (argument_ids);
		}

		for (char c : id) {
			std::string id_str = std::string(1, c);
			if (this->arguments.count(id_str) != 0 && this->arguments.at(id_str).get_flag() == GargoyleArgumentFlag::DASH) {
				argument_ids.push_back(id_str);
			}
		}

	}

	// Neither DASH nor DOUBLE_DASH
	else {
		if (this->arguments.count(argument) != 0 && this->arguments.at(argument).get_flag() == GargoyleArgumentFlag::NONE) {
			argument_ids.push_back(argument);
		}
		
	}
	

	return (argument_ids);
}

std::vector<std::string> Gargoyle::parse_arguments(std::vector<std::string> arguments) {
	std::vector<std::string> argument_ids = std::vector<std::string>();
	for (std::string arg : arguments) {
		std::vector<std::string> parsed_argument_ids = parse_argument(arg);
		for (std::string id : parsed_argument_ids) {
			argument_ids.push_back(id);
		}
	}

	return (argument_ids);

}

bool Gargoyle::add_argument(const GargoyleArgumentFlag flag, const std::string& id, bool& variable, std::string description, bool optional) {
	auto handler = [&variable](const std::string& ...) -> bool {
		variable = true;

		return true;
	};

	GargoyleArgument arg = GargoyleArgument(flag, id, description, optional, handler);
	return (this->add_argument(arg));
}

bool Gargoyle::add_argument(const GargoyleArgumentFlag flag, const std::string& id, std::function<void(const std::string&)> callback, std::string description, bool optional) {
	auto handler = [callback](const std::string& value) -> bool {
		if (value.empty()) {
			return (false);
		}

		try {
			callback(value);
		}
		catch (...) {
			return (false);
		}
		return (true);
	};

	GargoyleArgument arg = GargoyleArgument(flag, id, description, optional, handler);
	return (this->add_argument(arg));
}

bool Gargoyle::add_argument(const GargoyleArgumentFlag flag, const std::string& id, std::function<void()> callback, std::string description, bool optional) {
	auto handler = [callback](const std::string& ...) -> bool {
		try {
			callback();
		}
		catch (...) {
			return (false);
		}
		return (true);
	};

	GargoyleArgument arg = GargoyleArgument(flag, id, description, optional, handler);
	return (this->add_argument(arg));
}

bool Gargoyle::run() {
	std::vector<std::string> parsed_argument_ids = parse_arguments(this->raw_arguments);
	for (size_t i = 0; i < parsed_argument_ids.size(); i++) {
		std::string id = parsed_argument_ids.at(i);
		GargoyleArgument argument = this->arguments.at(id);
		if (i == parsed_argument_ids.size() - 1) {
			argument.run("");
		}
		else {
			argument.run(parsed_argument_ids[i + 1]);
		}
	}
	return (true);
}
