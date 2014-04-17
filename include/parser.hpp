#ifndef PARSER_HPP
#define PARSER_HPP

#include <map>

#include "ast.hpp"
#include "lexer.hpp"

class ParseException : public std::runtime_error {
public:
	ParseException(std::string msg) : std::runtime_error(msg) {}
};

class Parser {
public:
	Lexer lexer_;

	Token token;
	Token eat(std::string msg);

	int parseLine(std::string line);

	Expr *parseNumber();
	Expr *parseParen();
	Expr *parseBinOp();
	Expr *parseExpr();
};

#endif