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

	int eval(const Expr &expr) {
		return boost::apply_visitor(*this, expr);
	}

	int operator()(int i) {
		return i;
	}

	int operator()(const Call &call) {
		if (call.target_.type() == typeid(std::string)) {
			auto name = boost::get<std::string>(call.target_);

			// TODO handle builtins nicer
			if (name == "+") { // Builtins
				return eval(call.args_[0]) + eval(call.args_[1]);
			} else if (name == "-") {
				return eval(call.args_[0]) - eval(call.args_[1]);
			} else if (name == "*") {
				return eval(call.args_[0]) * eval(call.args_[1]);
			} else if (name == "/") {
				return eval(call.args_[0]) / eval(call.args_[1]);
			} else { // Argument
				return eval(env_.getArg(std::move(name)));
			}
		} else { // Definition
			passedArgs_ = std::vector<Expr>(call.args_);
			return eval(boost::get<DefPtr>(call.target_));
		}
	}

	int operator()(const DefPtr &def) {
		env_.pushArgs(def->params_, passedArgs_);
		int result = eval(def->body_);
		env_.popArgs();
		return result;
	}
};

#endif

