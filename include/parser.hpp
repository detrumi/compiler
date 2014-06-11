#ifndef PARSER_HPP
#define PARSER_HPP

#include <map>
#include <stack>
#include <set>
#include <stdexcept>

#include "ast.hpp"
#include "lexer.hpp"

struct Environment
{
	std::map<std::string, DefPtr> definitions_;
	std::stack<std::set<std::string>> paramStack_;
};

class ParseException : public std::runtime_error {
public:
	ParseException(std::string msg) : std::runtime_error(msg) {}
};

class Parser {
	const std::map<char, int> opPrecedence_ =
		{{'+',1},{'-',1},{'*',2},{'/',2}};

	Token token_;
	Lexer lexer_;
	Environment &env_;
public:
	Parser(Environment &env) : env_(env) {}

	Token getToken();

	Expr parseLine(std::string line);

	Expr parseNumber();
	Expr parseParen();
	Expr parseCall();
	Expr parseExpr(int prec = 0);
	Expr parsePrimary();
	Definition parseDef();
	Expr parseLambda();
};

#endif