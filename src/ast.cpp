#include <iostream>
#include <numeric>
#include "ast.hpp"
#include "environment.hpp"

int CallExpr::evaluate(Environment &env) {
	if (env.hasArgs()) {
		auto &args = env.getArgs();

		auto x = args.find(name_);
		if (x != args.end()) { // Evaluate argument
			return x->second->evaluate(env);
		}
	}

	if (name_ == "+") {
		return std::accumulate(std::begin(args_), std::end(args_), 0, [&](int res, ExprPtr &e){
			return res + e->evaluate(env);
		});
	} else if (name_ == "-") {
		return std::accumulate(std::begin(args_), std::end(args_), 0, [&](int res, ExprPtr &e) {
			return res - e->evaluate(env);
		});
	} else if (name_ == "*") {
		return std::accumulate(std::begin(args_), std::end(args_), 1, [&](int res, ExprPtr &e) {
			return res * e->evaluate(env);
		});
	} else if (name_ == "/") {
		return std::accumulate(std::begin(args_) + 1, std::end(args_), args_[0]->evaluate(env), [&](int res, ExprPtr &e) {
			int rhs = e->evaluate(env);
			if (rhs == 0) {
				throw CodegenException("Divide by 0 error");
			}
			return res / rhs;
		});
	} else {
		Definition &def = env.getDef(name_);
		return def.evaluateDef(env, args_);
	}
}
