#include "environment.hpp"

void Environment::addDefinition(Definition definition) {
	definitions_[definition.name_] = Expr(std::move(definition));
}

std::string Environment::addLambda(std::vector<std::string> params, Expr body) {
	std::string name = "#" + std::to_string(lambdaCounter_++);
	addDefinition(Definition(name, std::move(params), std::move(body)));
	return std::move(name);
}

Expr& Environment::getDefinition(std::string &name) {
	auto def = definitions_.find(name);
	if (def == definitions_.end()) {
		throw CodegenException("Undefined variable '" + name + "'");
	}
	return def->second;
}

void Environment::pushArgs(std::vector<std::string> &params, std::vector<Expr> &values) {
	std::map<std::string, Expr> argMap;
	for (size_t i = 0; i < params.size(); ++i) {
		argMap[params[i]] = std::move(values[i]);
	}
	arguments_.push(std::move(argMap));
}

void Environment::popArgs() {
	arguments_.pop();
}

boost::optional<Expr> Environment::getArg(std::string name) {
	if (arguments_.size() != 0) {
		auto &args = arguments_.top();

		auto x = args.find(name);
		if (x != args.end()) {
			return boost::optional<Expr>(x->second);
		}
	}
	return boost::optional<Expr>();
}