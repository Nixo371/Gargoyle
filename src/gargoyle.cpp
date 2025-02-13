#include "gargoyle.hpp"

#include <iostream>
#include <string>

Gargoyle::Gargoyle(int argc, char *argv[]) {
	for (int i = 1; i < argc; i++) {
		this->raw_arguments.push_back(argv[i]);
	}
}

bool Gargoyle::add_argument(GargoyleArgument argument) {
	std::string flag = argument.get_flag();
	if (this->arguments.count(flag) == 1) {
		std::cerr << "This argument is already registered!";
		return (false);
	}
	auto ret = this->arguments.insert({flag, argument});

	return (ret.second);
}

bool Gargoyle::add_argument(const std::string& flag, bool& variable, std::string description, bool optional) {
	auto handler = [&variable](const std::string& ...) -> bool {
		variable = true;

		return true;
	};

	GargoyleArgument arg = GargoyleArgument(flag, description, optional, handler);
	return (this->add_argument(arg));
}

bool Gargoyle::add_argument(const std::string& flag, std::function<void(const std::string&)> callback, std::string description, bool optional) {
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

	GargoyleArgument arg = GargoyleArgument(flag, description, optional, handler);
	return (this->add_argument(arg));
}

bool Gargoyle::add_argument(const std::string& flag, std::function<void()> callback, std::string description, bool optional) {
	auto handler = [callback](const std::string& ...) -> bool {
		try {
			callback();
		}
		catch (...) {
			return (false);
		}
		return (true);
	};

	GargoyleArgument arg = GargoyleArgument(flag, description, optional, handler);
	return (this->add_argument(arg));
}

bool Gargoyle::run() {
	for (size_t i = 0; i < this->raw_arguments.size(); i++) {
		std::string arg = this->raw_arguments[i];
		if (this->arguments.count(arg) == 0) {
			continue;
		}

		GargoyleArgument argument = this->arguments.at(arg);
		if (i == this->raw_arguments.size() - 1) {
			argument.run("");
		}
		else {
			argument.run(this->raw_arguments[i + 1]);
		}
	}
	return (true);
}
