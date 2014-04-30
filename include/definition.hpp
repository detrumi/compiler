#ifndef DEFINITION_HPP
#define DEFINITION_HPP

#include <string>
#include <vector>

#include "ast.hpp"

class Definition;

class Definition {
	std::string name_;
	std::vector<std::string> params_;
	ExprPtr body_;
public:
	Definition(ExprPtr body) : body_(std::move(body)) {}
	Definition(std::string name, std::vector<std::string> args, ExprPtr body)
		: name_(std::move(name)), params_(std::move(args)), body_(std::move(body)) {}

	std::string &getName() { return name_; }
	int paramCount() { return params_.size(); }

	int evaluateDef(Environment &env, std::vector<ExprPtr> &args);
};

#endif
