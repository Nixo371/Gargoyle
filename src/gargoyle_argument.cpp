#include "gargoyle_argument.hpp"

GargoyleArgument::GargoyleArgument(const std::string& flag,
				   const std::string& description,
				   bool optional,
				   std::function<bool(const std::string&)> handler) {
	this->flag = flag;
	this->description = description;
	this->optional = optional;
	this->handler = handler;
}

bool GargoyleArgument::run(const std::string& argument) {
	return (this->handler(argument));
}

std::string GargoyleArgument::get_flag() {
	return (this->flag);
}

std::string GargoyleArgument::get_description() {
	return (this->description);
}

bool GargoyleArgument::get_optional() {
	return (this->optional);
}
