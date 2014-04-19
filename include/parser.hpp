#ifndef PARSER_HPP
#define PARSER_HPP

#include <map>
#include <stdexcept>

#include "ast.hpp"
#include "lexer.hpp"
#include "definition.hpp"

class ParseException : public std::runtime_error {
public:
	ParseException(std::string msg) : std::runtime_error(msg) {}
};

class Parser {
public:
	Lexer lexer_;

	Token token;
	Token getToken();

	DefPtr parseLine(std::string line);

	ExprPtr parseNumber();
	ExprPtr parseParen();
	ExprPtr parseBinOp();
	ExprPtr parseExpr();
	DefPtr parseDef();
};

#endif