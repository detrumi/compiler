#include <string>

#include "lexer.hpp"

// Return next token from standard input
Token Lexer::getToken() {
	Token token;

	// Skip whitespace
	while (isspace(lastChar_)) {
		lastChar_ = getchar();
	}

	// Identifier: [a-zA-Z][a-zA-Z0-9]*
	if (isalpha(lastChar_)) {
		token.type = TokenType::tok_identifier;
		token.str = lastChar_;
		while (isalnum((lastChar_ = getchar()))) {
			token.str += lastChar_;
		}

		token.type = TokenType::tok_identifier;

		return token;
	}

	// Number: [0-9.]+
	if (isdigit(lastChar_)) {
		std::string numStr;
		do {
			numStr += lastChar_;
			lastChar_ = getchar();
		} while (isdigit(lastChar_));

		token.num = strtod(numStr.c_str(), 0);
		token.type = TokenType::tok_number;
		return token;
	}

	// Comment until end of line
	if (lastChar_ == '#') {
		do {
			lastChar_ = getchar();
		} while (lastChar_ != EOF && lastChar_ != '\n' && lastChar_ != '\r');
	}

	// Check for end of file, but leave it in lastChar_
	if (lastChar_ == EOF) {
		token.type = TokenType::tok_eof;
		return token;
	}

	// Return character as ascii value
	token.type = TokenType::tok_symbol;
	token.symbol = lastChar_;
	lastChar_ = getchar();
	return token;
}