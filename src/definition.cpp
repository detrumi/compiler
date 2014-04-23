#include <iostream>

#include "definition.hpp"
#include "environment.hpp"

int Definition::evaluateDef(Environment &env, std::vector<ExprPtr> &args) {
	env.pushArgs(args_, args);
	return body_->evaluate(env);
	env.popArgs();
}