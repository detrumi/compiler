#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include <vector>

#include "ast.hpp"

class Evaluator : public boost::static_visitor<int> {
	std::vector<std::string> parameters_;
	std::vector<Expr> arguments_;
public:
	int eval(const Expr &expr) {
		return boost::apply_visitor(*this, expr);
	}

	int operator()(int i) {
		return i;
	}

	int operator()(const DefPtr &def) {
		std::vector<std::string> params;

		std::swap(params, parameters_);
		int result = eval(def->body_);
		std::swap(params, parameters_);

		return result;
	}

	int operator()(const Lambda &lambda) {
		parameters_.insert(parameters_.end(), lambda.params_.begin(), lambda.params_.end());
		int result = eval(lambda.body_);
		parameters_.resize(parameters_.size() - lambda.params_.size());
		return result;
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
				for (int i = parameters_.size() - 1; i >= 0; --i) {
					if (parameters_[i] == name) {
						return eval(arguments_[i]);
					}
				}
				throw CodegenException("Unknown argument '" + name + "'");
			}
		} else { // Definition or lambda
			int result;
			std::vector<Expr> args = std::vector<Expr>(call.args_);
			
			std::swap(args, arguments_);
			if (call.target_.type() == typeid(Lambda)) {
				result = eval(boost::get<Lambda>(call.target_));
			} else {
				result = eval(boost::get<DefPtr>(call.target_));
			}
			std::swap(arguments_, args);
			return result;
		}
	}
};

#endif

