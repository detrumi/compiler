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
	std::map<char, int> binOpPrecedence_ =
		{{'+',20},{'-',20},{'*',40}};

	Lexer lexer_;
public:
	Token token;
	Token getNextToken();

	Expr *parseNumber();
	Expr *parseParen();
	Expr *parsePrimary();
	Expr *parseBinOpRhs(int exprPrec, Expr *lhs);
	Expr *parseExpr();
	Function *parseDefinition();
	Function *parseTopLevelExpr();
};

#endif