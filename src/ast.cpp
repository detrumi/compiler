#include <iostream>
#include "ast.hpp"
#include "environment.hpp"

int BinaryExpr::evaluate(Environment &env) {
	switch (op_) {
		case '+': return lhs_->evaluate(env) + rhs_->evaluate(env);
		case '-': return lhs_->evaluate(env) - rhs_->evaluate(env);
		case '*': return lhs_->evaluate(env) * rhs_->evaluate(env);
		case '/': {
			int rhsVal = rhs_->evaluate(env);
			if (rhsVal == 0)
				throw CodegenException("Divide by 0 error");
			return lhs_->evaluate(env) / rhsVal;
		}
		default: throw CodegenException("Unknown binary operator");
	}
}

int VarExpr::evaluate(Environment &env) {
	return env[name_]->evaluateDef(env);
}