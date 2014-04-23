#include <iostream>

#include "definition.hpp"
#include "environment.hpp"

std::string &Definition::getName() {
	return name_;
}

int Definition::evaluateDef(Environment &env, std::vector<ExprPtr> &args) {
	env.pushArgs(argNames_, args);
	return body_->evaluate(env);
	env.popArgs();
}