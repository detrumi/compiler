#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <map>
#include <stack>
#include <iostream>
#include <algorithm>

#include "definition.hpp"

class Environment
{
	std::map<std::string, DefPtr> definitions_;
	std::stack<std::map<std::string, ExprPtr>> arguments_;
public:
	void addDefinition(DefPtr definition) {
		std::string name = definition->getName();
		definitions_[name] = std::move(definition);
	}

	void pushArgs(std::vector<std::string> &argNames, std::vector<ExprPtr> &values) {
		std::map<std::string, ExprPtr> argMap;
		for (size_t i = 0; i < argNames.size(); ++i) {
			argMap[argNames[i]] = std::move(values[i]);
		}
		arguments_.push(std::move(argMap));
	}

	void popArgs() {
		arguments_.pop();
	}

	Definition& getDef(std::string &name) {
		auto def = definitions_.find(name);
		if (def == definitions_.end()) {
			throw CodegenException("Undefined variable '" + name + "'");
		}
		return *def->second;
	}

	bool hasArgs() {
		return arguments_.size() != 0;
	}

	std::map<std::string, ExprPtr> &getArgs() {
		return arguments_.top();
	}
};

#endif