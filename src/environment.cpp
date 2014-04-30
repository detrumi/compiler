#include "environment.hpp"

void Environment::addDefinition(Definition definition) {
	std::string name = definition.getName();
	definitions_.erase(name); // TODO replace instead of remove and add
	definitions_.emplace(std::make_pair(name, std::move(definition)));
}

Definition& Environment::getDefinition(std::string &name) {
	auto def = definitions_.find(name);
	if (def == definitions_.end()) {
		throw CodegenException("Undefined variable '" + name + "'");
	}
	return def->second;
}

void Environment::pushArgs(std::vector<std::string> &argNames, std::vector<ExprPtr> &values) {
	std::map<std::string, ExprPtr> argMap;
	for (size_t i = 0; i < argNames.size(); ++i) {
		argMap[argNames[i]] = std::move(values[i]);
	}
	arguments_.push(std::move(argMap));
}

void Environment::popArgs() {
	arguments_.pop();
}

ExprPtr *Environment::getArg(std::string name) {
	if (arguments_.size() != 0) {
		auto &args = arguments_.top();

		auto x = args.find(name);
		if (x != args.end()) {
			return &x->second;
		}
	}
	return nullptr;
}