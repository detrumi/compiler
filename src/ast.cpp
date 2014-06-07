#include <iostream>
#include <numeric>
#include "ast.hpp"
#include "environment.hpp"

bool Expr::expectArg() {
	if (argsExpected_ > 0) {
		argsExpected_--;
		return true;
	}
	return false;
}

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

void CallExpr::addArg(ExprPtr arg) {
	args_.push_back(std::move(arg));
}

int LambdaExpr::evaluate(Environment &env) {
	env.pushArgs(params_, args_);
	int result = body_->evaluate(env);
	env.popArgs();
	return result;
}

void LambdaExpr::addArg(ExprPtr arg) {
	args_.push_back(std::move(arg));
}