#include <iostream>
#include "parser.hpp"

void mainLoop(Parser &parser) {
//	Lexer lexer;
	while (1) {
		fprintf(stderr, "ready> ");
		std::string line;
		std::getline(std::cin, line);
//		lexer.setLine(line);
//		for (int i = 0; i < 10; i++) {
//			std::cout << lexer.getToken().symbol << std::endl;
//		}
		std::cout << "Result: " << parser.parseLine(line) << std::endl;
	}
}

int main() {
	Parser parser;
	mainLoop(parser);
}