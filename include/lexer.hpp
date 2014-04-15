#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>

enum class TokenType {
	tok_eof, tok_identifier, tok_symbol, tok_number,
};

struct Token {
	TokenType type;
	std::string str;
	int num;
	char symbol;
};

class Lexer {
	std::string line_;
	int index_;
public:
	char nextChar();
	void setLine(std::string line);
	Token getToken();
};

#endif