#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <map>
#include <stack>

#include "definition.hpp"

class Environment
{
	std::map<std::string, DefPtr> definitions_;
	std::stack<std::map<std::string, ExprPtr>> arguments_;
public:
	void addDefinition(DefPtr definition);
	void pushArgs(std::vector<std::string> &argNames, std::vector<ExprPtr> &values);
	void popArgs();
	Definition& getDef(std::string &name);
	bool hasArgs();
	std::map<std::string, ExprPtr> &getArgs();
};

#endif