#include <string>
#include <iostream>

#include "lexer.hpp"

void Lexer::setLine(std::string line) {
	line_ = line;
	index_ = 0;
}

// Return next token from standard input
Token Lexer::getToken() {
	Token token;

	// Skip whitespace
	while (isspace(line_[index_])) {
		index_++;
	}

	// Identifier: [a-zA-Z][a-zA-Z0-9]*
	if (isalpha(line_[index_])) {
		token.type = TokenType::tok_identifier;
		token.str = line_[index_++];
		while (isalnum(line_[index_])) {
			token.str += line_[index_++];
		}

		token.type = TokenType::tok_identifier;
	
		std::cout << "Lexed identifier: " << token.str << std::endl;
		return token;
	}

	// Number: [0-9.]+
	if (isdigit(line_[index_])) {
		std::string numStr;
		do {
			numStr += line_[index_++];
		} while (isdigit(line_[index_]));

		token.num = strtol(numStr.c_str(), 0, 10);
		token.type = TokenType::tok_number;

		std::cout << "Lexed number: " << numStr << std::endl;
		return token;
	}

	// Comment until end of line
	if (line_[index_] == '#') {
		do {
			index_++;
		} while (line_[index_] != EOF && line_[index_] != '\n' && line_[index_] != '\r');
	}

	// Check for end of file, but leave it in line_
	if (line_[index_] == EOF) {
		token.type = TokenType::tok_eof;
		return token;
	}

	// Return character as ascii value
	token.type = TokenType::tok_symbol;
	token.symbol = line_[index_++];
	std::cout << "Lexed symbol: " << token.symbol << std::endl;
	return token;
}