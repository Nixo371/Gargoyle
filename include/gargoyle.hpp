#ifndef GARGOYLE_HPP
# define GARGOYLE_HPP

# include <unordered_map>
# include <string>
# include <functional>

# include "gargoyle_argument.hpp"

/**
 * @brief The main class that manages and executes arguments
 */
class Gargoyle {
	private:
		std::vector<std::string> raw_arguments;
		std::unordered_map<std::string, GargoyleArgument> arguments;


		bool add_argument(GargoyleArgument argument);
	public:
		/**
		 * @brief The class constructor
		 *
		 * @param argc The argc argument from the main function
		 * @param argv The argv argument from the main function
		 */
		Gargoyle(int argc, char *argv[]);

		/**
		 * @brief Adds an argument that sets a boolean variable to true if present
		 *
		 * @param flag The string to search for in the arguments e.g. ("-v", "--version")
		 * @param variable The boolean variable to set to true if the flag exists
		 * @param description [Optional] Set a description for the argument - Default = ""
		 * @param optional [Optional] Determines if the argument is optional for the program to run - Default = true
		 *
		 * @return A boolean value that determines if adding the argument was successful.
		 */
		bool add_argument(const std::string& flag, bool& variable, std::string description = "", bool optional = true);

		/**
		 * @brief Adds an argument that runs a function with an additional argument
		 *
		 * @param flag The string to search for in the arguments e.g. ("-t", "--time")
		 * @param callback The lambda function to run with the next argument as a string in line
		 * @param description [Optional] Set a description for the argument - Default = ""
		 * @param optional [Optional] Determines if the argument is optional for the program to run - Default = true
		 *
		 * @return A boolean value that determines if adding the argument was successful.
		 */
		bool add_argument(const std::string& flag, std::function<void(const std::string&)> callback, std::string description = "", bool optional = true);
		
		/**
		 * @brief Adds an argument that runs a function without an additional argument
		 *
		 * @param flag The string to search for in the arguments e.g. ("-v", "--verbose")
		 * @param callback The lambda function to run
		 * @param description [Optional] Set a description for the argument - Default = ""
		 * @param optional [Optional] Determines if the argument is optional for the program to run - Default = true
		 *
		 * @return A boolean value that determines if adding the argument was successful.
		 */
		bool add_argument(const std::string& flag, std::function<void()> callback, std::string description = "", bool optional = true);

		/**
		 * @brief Checks if any arguments were present and runs their execution if applicable
		 */
		bool run();
};

#endif
