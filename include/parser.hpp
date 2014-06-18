#ifndef PARSER_HPP
#define PARSER_HPP

#include <map>
#include <set>
#include <stdexcept>

#include "ast.hpp"
#include "lexer.hpp"

struct Environment
{
	std::map<std::string, DefPtr> definitions_;
	std::vector<std::string> params_;
};

class ParseException : public std::runtime_error {
public:
	ParseException(std::string msg) : std::runtime_error(msg) {}
};

class Parser {
	const std::map<char, int> opPrecedence_ =
		{{'+',1},{'-',1},{'*',2},{'/',2}};

	Environment &env_;
	Token token_;
	Lexer lexer_;
public:
	Parser(Environment &env)
		: env_(env), token_() {}

	Token getToken();

	Expr parseLine(std::string line);

	Expr parseNumber();
	Expr parseParen();
	Expr parseIdentifier();
	Expr parseExpr(int prec = 0);
	Expr parsePrimary();
	Definition parseDef();
	Expr parseLambda();
};

#endif