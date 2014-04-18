#include <iostream>
#include "parser.hpp"

void mainLoop(Parser &parser) {
	while (std::cin) {
		try {
			std::cout << ">>> ";

			std::string line;
			std::getline(std::cin, line);
			if (line.length() > 0) {
				std::cout << parser.parseLine(line) << std::endl;
			}
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
	std::cout << std::endl;
}