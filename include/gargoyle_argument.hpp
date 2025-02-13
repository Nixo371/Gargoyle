#ifndef GARGOYLE_ARGUMENT_HPP
# define GARGOYLE_ARGUMENT_HPP

# include <string>
# include <functional>

class GargoyleArgument {
	private:
		std::string flag;
		std::string description;
		bool optional;
		std::function<bool(const std::string&)> handler;
	public:
		GargoyleArgument(const std::string& flag,
				 const std::string& description,
				 bool optional,
				 std::function<bool(const std::string&)> handler);
		
		bool run(const std::string& argument);

		std::string get_flag();
		std::string get_description();
		bool get_optional();
};

#endif
