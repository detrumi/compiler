#include <iostream>
#include "parser.hpp"
#include "environment.hpp"

int main() {
	Parser parser;
	Environment env;
	while (std::cin) {
		try {
			std::cout << ">>> ";

			std::string line;
			std::getline(std::cin, line);
			if (line.length() > 0) {
				DefPtr def = parser.parseLine(line);
				if (def->getName() == "") { // Not a function; directly print answer
					std::cout << def->evaluateDef(env) << std::endl;
				} else {
					env.addDefinition(std::move(def));
				}
			}
		} catch (ParseException ex) {
			std::cout << ex.what() << std::endl;
		} catch (CodegenException ex) {
			std::cout << ex.what() << std::endl;
		}
	}
	std::cout << std::endl;
}