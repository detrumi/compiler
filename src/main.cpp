#include <iostream>
#include "parser.hpp"

int main() {
	Parser parser;
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
	std::cout << std::endl;
}