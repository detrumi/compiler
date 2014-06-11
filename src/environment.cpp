#include "environment.hpp"

DefPtr Environment::addLambda(std::vector<std::string> params, Expr body) {
	return std::make_shared<Definition>("", std::move(params), std::move(body));
}

void Environment::addDefinition(Definition definition) {
	definitions_[definition.name_] = std::move(std::make_shared<Definition>(definition));
}

DefPtr Environment::getDefinition(std::string name) {
	auto def = definitions_.find(name);
	if (def == definitions_.end()) {
		throw CodegenException("Undefined variable '" + name + "'");
	}
	return def->second;
}

void Environment::pushArgs(const std::vector<std::string> &params, std::vector<Expr> &values) {
	std::map<std::string, Expr> argMap;
	for (size_t i = 0; i < params.size(); ++i) {
		argMap[params[i]] = std::move(values[i]);
	}
	arguments_.push(std::move(argMap));
}

void Environment::popArgs() {
	arguments_.pop();
}

Expr &Environment::getArg(std::string name) {
	if (arguments_.size() != 0) {
		auto &args = arguments_.top();

		auto x = args.find(name);
		if (x != args.end()) {
			return x->second;
		}
	}
	throw CodegenException("Unknown argument '" + name + "'");
}