#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <map>
#include <stack>
#include <boost/optional.hpp>

#include "ast.hpp"

class Environment
{
	std::map<std::string, Expr> definitions_;
	std::stack<std::map<std::string, Expr>> arguments_;
public:
	void addDefinition(Definition definition);
	Expr& getDefinition(std::string &name);

	void pushArgs(std::vector<std::string> &params, std::vector<Expr> &values);
	void popArgs();

	boost::optional<Expr> getArg(std::string name);
};

#endif