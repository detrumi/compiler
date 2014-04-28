#include <iostream>
#include <numeric>
#include "ast.hpp"
#include "environment.hpp"

int CallExpr::evaluate(Environment &env) {
	if (auto arg = env.getArg(name_)) { // Evaluate argument
		return (*arg)->evaluate(env);
	}

	if (name_ == "+") {
		return args_[0]->evaluate(env) + args_[1]->evaluate(env);
	} else if (name_ == "-") {
		return args_[0]->evaluate(env) - args_[1]->evaluate(env);
	} else if (name_ == "*") {
		return args_[0]->evaluate(env) * args_[1]->evaluate(env);
	} else if (name_ == "/") {
		return args_[0]->evaluate(env) / args_[1]->evaluate(env);
	} else {
		Definition &def = env.getDefinition(name_);
		return def.evaluateDef(env, args_);
	}
}
