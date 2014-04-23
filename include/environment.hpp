#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <map>
#include <iostream>

#include "definition.hpp"

class Environment
{
	std::map<std::string, DefPtr> definitions_;
public:
	void addDefinition(DefPtr definition) {
		std::string name = definition->getName();
		definitions_[name] = std::move(definition);
	}

	Definition& getDef(std::string &name) {
		auto def = definitions_.find(name);
		if (def == definitions_.end()) {
			throw CodegenException("Undefined variable '" + name + "'");
		}
		return *def->second;
	}
};

#endif