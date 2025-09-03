#include "gargoyle_argument.hpp"

GargoyleArgument::GargoyleArgument(const GargoyleArgumentFlag flag,
				   const std::string& id,
				   const std::string& description,
				   bool optional,
				   std::function<bool(const std::string&)> handler) {
	this->flag = flag;
	this->id = id;
	this->description = description;
	this->optional = optional;
	this->handler = handler;
}

bool GargoyleArgument::run(const std::string& argument) {
	return (this->handler(argument));
}

GargoyleArgumentFlag GargoyleArgument::get_flag() {
	return (this->flag);
}

std::string GargoyleArgument::get_id() {
	return (this->id);
}

std::string GargoyleArgument::get_description() {
	return (this->description);
}

bool GargoyleArgument::get_optional() {
	return (this->optional);
}

std::string GargoyleArgument::get_full_id() {
	switch (this->get_flag()) {
		case GargoyleArgumentFlag::NONE:
			return (this->get_id());
		case GargoyleArgumentFlag::DASH:
			return ("-" + this->get_id());
		case GargoyleArgumentFlag::DOUBLE_DASH:
			return ("--" + this->get_id());
	}
	return ("");
}
