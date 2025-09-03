#include "gargoyle.hpp"
#include "gargoyle_argument.hpp"

#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

Gargoyle::Gargoyle(int argc, char **argv, bool default_help) {
	this->default_help = default_help;
	for (int i = 0; i < argc; i++) {
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

std::vector<ParsedArgument> Gargoyle::parse_argument(std::string argument) {
	std::vector<ParsedArgument> argument_ids = std::vector<ParsedArgument>();
	// DOUBLE_DASH
	if (argument.substr(0, 2) == "--") {
		std::string id = argument.substr(2);
		if (this->arguments.count(id) != 0 && this->arguments.at(id).get_flag() == GargoyleArgumentFlag::DOUBLE_DASH) {
			ParsedArgument parsed_argument;
			parsed_argument.argument_id = id;
			parsed_argument.optional_argument = "NEXT";
			argument_ids.push_back(parsed_argument);
		}
	}
	// TODO How do I handle multi-character single dash arguments?
	// Is "-qwer" 4 arguments? 1?
	// For now I'll assume it's either 1 argument (check the whole string) and otherwise it's a collection of 1 character arguments
	else if (argument.substr(0, 1) == "-") {
		// First check for multi character id
		std::string id = argument.substr(1);
		if (this->arguments.count(id) != 0 && this->arguments.at(id).get_flag() == GargoyleArgumentFlag::DASH) {
			ParsedArgument parsed_argument;
			parsed_argument.argument_id = id;
			parsed_argument.optional_argument = "NEXT";
			argument_ids.push_back(parsed_argument);

			return (argument_ids);
		}

		for (char c : id) {
			std::string id_str = std::string(1, c);
			if (this->arguments.count(id_str) != 0 && this->arguments.at(id_str).get_flag() == GargoyleArgumentFlag::DASH) {
				ParsedArgument parsed_argument;
				parsed_argument.argument_id = id_str;
				parsed_argument.optional_argument = "";
				argument_ids.push_back(parsed_argument);
			}
		}

	}

	// Neither DASH nor DOUBLE_DASH
	else {
		if (this->arguments.count(argument) != 0 && this->arguments.at(argument).get_flag() == GargoyleArgumentFlag::NONE) {
			ParsedArgument parsed_argument;
			parsed_argument.argument_id = argument;
			parsed_argument.optional_argument = "NEXT";
			argument_ids.push_back(parsed_argument);
		}
	}
	

	return (argument_ids);
}

std::vector<ParsedArgument> Gargoyle::parse_arguments(std::vector<std::string> arguments) {
	std::vector<ParsedArgument> parsed_arguments = std::vector<ParsedArgument>();
	for (size_t i = 0; i < arguments.size(); i++) {
		std::string arg = arguments.at(i);

		std::vector<ParsedArgument> parsed_argument = parse_argument(arg);
		for (ParsedArgument parsed_arg : parsed_argument) {
			// This prevents stuff like './program -abc input.txt' as being interpreted as each of a, b, and c having the optional argument of "input.txt"
			// If there is not one argument found (-abc) it will interpret single letters, which won't have an argument attached to them, forcing the use of '-ab -c input.txt'
			if (parsed_arg.optional_argument == "NEXT") {
				if (i + 1 < arguments.size()) {
					parsed_arg.optional_argument = arguments.at(i + 1);
				}
				else {
					parsed_arg.optional_argument = "";
				}
			}
			parsed_arguments.push_back(parsed_arg);
		}
	}

	return (parsed_arguments);
}

void Gargoyle::register_help_argument() {
	std::string SPACER = "\t\t";

	std::stringstream ss = std::stringstream();
	ss << "Usage:\n";
	ss << "  " << this->raw_arguments[0] << " [options]\n";
	ss << "\n";
	ss << "Options:\n";

	ss << "  " << "-h, --help" << SPACER << "Print this help message" << "\n";
	for (std::pair<std::string, GargoyleArgument> arg : this->arguments) {
		std::string id = arg.first;
		GargoyleArgument argument = arg.second;

		ss << "  " << argument.get_full_id() << SPACER << argument.get_description() << "\n";
	}
	std::string help_message = ss.str();

	auto help = [help_message]() {
		std::cout << help_message << std::endl;
	};
	
	// TODO Should I delete any existing -h/--help?
	this->add_argument(GargoyleArgumentFlag::DASH, "h", help);
	this->add_argument(GargoyleArgumentFlag::DOUBLE_DASH, "help", help);
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
	this->register_help_argument();
	std::vector<std::string> arguments = std::vector<std::string>();
	arguments.insert(arguments.end(), this->raw_arguments.begin() + 1, this->raw_arguments.end());

	std::vector<ParsedArgument> parsed_arguments = parse_arguments(arguments);
	for (ParsedArgument parsed_argument : parsed_arguments) {
		std::string id = parsed_argument.argument_id;
		std::string optional_argument = parsed_argument.optional_argument;
		if (this->arguments.count(id) == 0) {
			std::cerr << "[ERROR] Could not find argument '" << id << "'" << std::endl;
			continue;
		}
		GargoyleArgument argument = this->arguments.at(id);
		
		argument.run(optional_argument);
	}

	return (true);
}
