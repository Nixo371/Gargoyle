#ifndef GARGOYLE_HPP
# define GARGOYLE_HPP

# include <unordered_map>
# include <string>
# include <functional>

# include "gargoyle_argument.hpp"

class Gargoyle {
	private:
		std::vector<std::string> raw_arguments;
		std::unordered_map<std::string, GargoyleArgument> arguments;


		// Adds to the actual map of arguments
		bool add_argument(GargoyleArgument argument);
	public:
		Gargoyle(int argc, char *argv[]);

		// Set a variable flag
		bool add_argument(const std::string& flag, bool& variable, std::string description = "", bool optional = true);

		// Custom handling of an argument passed in after the flag
		bool add_argument(const std::string& flag, std::function<void(const std::string&)> callback, std::string description = "", bool optional = true);
		
		// Custom handling of a flag
		bool add_argument(const std::string& flag, std::function<void()> callback, std::string description = "", bool optional = true);

		// Executes all the arguments
		bool run();
};

#endif
