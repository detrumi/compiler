#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include <vector>

#include "ast.hpp"
#include "environment.hpp"

class Evaluator : public boost::static_visitor<int> {
	Environment &env_;
	std::vector<Expr> passedArgs_;
public:
	Evaluator(Environment &env) : env_(env) {}

	int eval(Expr &expr) {
		return boost::apply_visitor(*this, expr);
	}

	int operator()(int i) {
		return i;
	}

	int operator()(Call &call) {
		if (auto arg = env_.getArg(call.name_)) { // Evaluate argument
			return eval(*arg);
		}

		if (call.name_ == "+") {
			return eval(call.args_[0]) + eval(call.args_[1]);
		} else if (call.name_ == "-") {
			return eval(call.args_[0]) - eval(call.args_[1]);
		} else if (call.name_ == "*") {
			return eval(call.args_[0]) * eval(call.args_[1]);
		} else if (call.name_ == "/") {
			return eval(call.args_[0]) / eval(call.args_[1]);
		} else {
			passedArgs_.clear();
			passedArgs_.insert(passedArgs_.begin(), call.args_.begin(), call.args_.end());

			auto &def = env_.getDefinition(call.name_);
			return eval(def);
		}
	}

	int operator()(Lambda &lambda) {
		env_.pushArgs(lambda.params_, lambda.args_);
		int result = eval(lambda.body_);
		env_.popArgs();
		return result;
	}

	int operator()(Definition &def) {
		env_.pushArgs(def.params_, passedArgs_);
		int result = eval(def.body_);
		env_.popArgs();
		return result;
	}
};

#endif

