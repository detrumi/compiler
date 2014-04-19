#include <iostream>

#include "definition.hpp"
#include "environment.hpp"

int Definition::evaluateDef(Environment &env) {
	return body_->evaluate(env);
}