#ifndef PRETTYPRINT_HPP
#define PRETTYPRINT_HPP

#include "ast.hpp"

class PrettyPrint : public boost::static_visitor<std::string> {
public:
	std::string print(const Expr &expr) {
		return boost::apply_visitor(*this, expr);
	}

	std::string operator()(int i) {
		return std::to_string(i);
	}

	std::string operator()(const DefPtr &def) {
		std::string result = "(";
		result += "\\";
		for (auto param : def->params_) {
			result += param + " ";
		}
		if (!def->params_.empty()) {
			result.pop_back(); // Remove last space
			result += ". ";
		}
		result += print(def->body_);
		result += ")";
		return result;
	}

	std::string operator()(const Call &call) {
		std::string result;
		if (!call.args_.empty()) {
			result += "(";
		}
		if (call.target_.type() == typeid(std::string)) {
			result += boost::get<std::string>(call.target_);
		} else {
			result += print(boost::get<DefPtr>(call.target_));
		}
		for (auto arg : call.args_) {
			result += " " + print(arg);
		}
		if (!call.args_.empty()) {
			result += ")";
		}
		return result;
	}

};

#endif