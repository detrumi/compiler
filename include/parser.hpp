#ifndef PARSER_HPP
#define PARSER_HPP

#include <map>
#include <stack>
#include <set>
#include <stdexcept>

#include "ast.hpp"
#include "lexer.hpp"

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

	void parseArgs(int argCount, std::vector<Expr> &target);
public:
	Parser(Environment &env) : env_(env) {}

	Token token;
	Token getToken();

	void parseLine(std::string line);

	Expr parseNumber();
	Expr parseParen();
	Expr parseCall();
	Expr parseExpr(int prec = 0);
	Expr parsePrimary();
	Definition parseDef();
	Expr parseLambda();
};

#endif