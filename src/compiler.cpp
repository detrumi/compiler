#include <iostream>
#include "parser.hpp"

void mainLoop(Parser &parser) {
	while (1) {
		try {
			fprintf(stderr, "ready> ");
			std::string line;
			std::getline(std::cin, line);
			std::cout << "Result: " << parser.parseLine(line) << std::endl;
		} catch (ParseException ex) {
			std::cout << ex.what() << std::endl;
		} catch (CodegenException ex) {
			std::cout << ex.what() << std::endl;
		}
	}
}

int main() {
	Parser parser;
	mainLoop(parser);
}