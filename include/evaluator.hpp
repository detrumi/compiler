#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include <vector>

#include "ast.hpp"

class Evaluator : public boost::static_visitor<int> {
	std::stack<std::map<std::string, Expr>> arguments_;
	std::vector<Expr> passedArgs_;
public:
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
				if (arguments_.size() != 0) {
					auto &args = arguments_.top();

					auto x = args.find(name);
					if (x != args.end()) {
						return eval(x->second);
					}
				}
				throw CodegenException("Unknown argument '" + name + "'");
			}
		} else { // Definition
			passedArgs_ = std::vector<Expr>(call.args_);
			return eval(boost::get<DefPtr>(call.target_));
		}
	}

	int operator()(const DefPtr &def) {
		std::map<std::string, Expr> argMap;
		for (size_t i = 0; i < def->params_.size(); ++i) {
			argMap[def->params_[i]] = std::move(passedArgs_[i]);
		}

		arguments_.push(std::move(argMap));
		int result = eval(def->body_);
		arguments_.pop();

		return result;
	}
};

#endif

