#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>

enum class TokenType {
	eof, endl, identifier, symbol, number,
};

struct Token {
	TokenType type;
	std::string str;
	int num;
};

class Lexer {
	std::string line_;
	int index_;
public:
	void setLine(std::string line);
	Token getToken();
};

#endif