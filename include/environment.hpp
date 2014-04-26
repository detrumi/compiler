#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <map>
#include <stack>

#include "definition.hpp"

class Environment
{
	std::map<std::string, Definition> definitions_;
	std::stack<std::map<std::string, ExprPtr>> arguments_;
public:
	void addDefinition(Definition definition);
	Definition& getDefinition(std::string &name);

	void pushArgs(std::vector<std::string> &argNames, std::vector<ExprPtr> &values);
	void popArgs();

	ExprPtr *getArg(std::string name);
};

#endif