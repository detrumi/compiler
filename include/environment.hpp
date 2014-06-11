#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <map>
#include <stack>

#include "ast.hpp"

class Environment
{
	std::map<std::string, DefPtr> definitions_;

	std::stack<std::map<std::string, Expr>> arguments_;
public:
	DefPtr addLambda(std::vector<std::string> params, Expr body);
	void addDefinition(Definition definition);
	DefPtr getDefinition(std::string name);

	void pushArgs(const std::vector<std::string> &params, std::vector<Expr> &values);
	void popArgs();

	Expr &getArg(std::string name);
};

#endif