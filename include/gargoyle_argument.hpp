#ifndef GARGOYLE_ARGUMENT_HPP
# define GARGOYLE_ARGUMENT_HPP

# include <string>
# include <functional>

enum GargoyleArgumentFlag {
	NONE,
	DASH,
	DOUBLE_DASH
};

class GargoyleArgument {
	private:
		GargoyleArgumentFlag flag;
		std::string id;
		std::string description;
		bool optional;
		std::function<bool(const std::string&)> handler;
	public:
		GargoyleArgument(const GargoyleArgumentFlag flag,
				 const std::string& id,
				 const std::string& description,
				 bool optional,
				 std::function<bool(const std::string&)> handler);
		
		bool run(const std::string& argument);

		GargoyleArgumentFlag get_flag();
		std::string get_id();
		std::string get_description();
		bool get_optional();

		std::string get_full_id();
};

#endif
