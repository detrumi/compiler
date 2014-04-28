#ifndef PARSER_HPP
#define PARSER_HPP

#include <map>
#include <stack>
#include <set>
#include <stdexcept>

#include "ast.hpp"
#include "lexer.hpp"
#include "definition.hpp"

class ParseException : public std::runtime_error {
public:
	ParseException(std::string msg) : std::runtime_error(msg) {}
};

class Parser {
	const std::map<char, int> opPrecedence_ =
		{{'+',1},{'-',1},{'*',2},{'/',2}};

	Lexer lexer_;
	Environment &env_;
	std::stack<std::set<std::string>> paramStack_;
public:
	Parser(Environment &env) : env_(env) {}

	Token token;
	Token getToken();

	void parseLine(std::string line);

	ExprPtr parseNumber();
	ExprPtr parseParen();
	ExprPtr parseCall();
	ExprPtr parseExpr(int prec = 0);
	ExprPtr parsePrimary();
	Definition parseDef();
};

#endif