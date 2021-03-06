#include <iostream>
#include "parser.hpp"
#include "evaluator.hpp"
#include "prettyPrint.hpp"

int main() {
	Environment env;
	Parser parser(env);
	while (std::cin) {
		try {
			std::cout << ">>> ";

			std::string line;
			std::getline(std::cin, line);
			if (line.length() > 0) {
				Expr expr = parser.parseLine(line);

				PrettyPrint printer;
				std::cout << printer.print(expr) << std::endl;

				if (expr.type() == typeid(DefPtr)) { // Definition
					auto def = boost::get<DefPtr>(std::move(expr));
					env.definitions_[def->name_] = def;
				} else { // Top-level expression
					Evaluator evaluator;
					std::cout << evaluator.eval(expr) << std::endl;
				}
				parser.parseLine(line);
			}
		} catch (ParseException &ex) {
			std::cout << ex.what() << std::endl;
		} catch (CodegenException &ex) {
			std::cout << ex.what() << std::endl;
		}
	}
	std::cout << std::endl;
}