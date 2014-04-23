#include "environment.hpp"

void Environment::addDefinition(DefPtr definition) {
	std::string name = definition->getName();
	definitions_[name] = std::move(definition);
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

Definition& Environment::getDef(std::string &name) {
	auto def = definitions_.find(name);
	if (def == definitions_.end()) {
		throw CodegenException("Undefined variable '" + name + "'");
	}
	return *def->second;
}

bool Environment::hasArgs() {
	return arguments_.size() != 0;
}

std::map<std::string, ExprPtr> &Environment::getArgs() {
	return arguments_.top();
}
