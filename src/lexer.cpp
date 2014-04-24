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

	if (line_[index_] == '\n') {
		token.type = TokenType::endl;
		return token;
	}

	// Skip whitespace
	while (isspace(line_[index_])) {
		index_++;
	}

	// Identifier: [a-zA-Z][a-zA-Z0-9]*
	if (isalpha(line_[index_])) {
		token.type = TokenType::identifier;
		token.str = line_[index_++];
		while (isalnum(line_[index_])) {
			token.str += line_[index_++];
		}

		token.type = TokenType::identifier;
	
		return token;
	}

	// Number: [0-9.]+
	if (isdigit(line_[index_])) {
		std::string numStr;
		do {
			numStr += line_[index_++];
		} while (isdigit(line_[index_]));

		token.num = strtol(numStr.c_str(), 0, 10);
		token.type = TokenType::number;

		return token;
	}

	// Comment until end of line
	if (line_[index_] == '#') {
		do {
			index_++;
		} while (line_[index_] != EOF && line_[index_] != '\n');
		token.type = TokenType::endl;
		return token;
	}

	// Check for end of file, but leave it in line_
	if (line_[index_] == EOF) {
		token.type = TokenType::eof;
		return token;
	}

	// Return character as ascii value
	token.type = TokenType::symbol;
	token.str = line_[index_++];
	return token;
}