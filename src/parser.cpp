#include "parser.hpp"
#include <iostream>

int Parser::parseLine(std::string line) {
	std::cout << "parseLine()" << std::endl;
	lexer_.setLine(line);
	eat("first token");
	Expr *expr = parseExpr();
	return expr->evaluate();
}

// Token buffer
Token Parser::eat(std::string msg) {
	std::cout << "Eat " << msg << std::endl;
	token = lexer_.getToken();
	return token;
}


Expr *Parser::parseNumber() {
	std::cout << "parseNumber()" << std::endl;
	Expr *result = new NumberExpr(token.num);
	eat("number");
	return result;
}

// (expr)
Expr *Parser::parseParen() {
	std::cout << "parseParen()" << std::endl;
	eat("(");

	Expr *expr = parseExpr();

	if (token.symbol != ')')
		throw ParseException("Expected ')'");
	eat(")");
	
	return expr;
}

Expr *Parser::parseBinOp() {
	std::cout << "parseBinOp()" << std::endl;

	char op = token.symbol;
	if (op != '+' && op != '-' && op != '*' && op != '/') {
		throw ParseException("Unknown binary operator");
	}
	eat("op");

	Expr *lhs = parseExpr();
	Expr *rhs = parseExpr();

	return new BinaryExpr(op, lhs, rhs);
}

Expr *Parser::parseExpr() {
	std::cout << "parseExpr()" << std::endl;
	switch (token.type) {
		case TokenType::tok_number:
			return parseNumber();
		case TokenType::tok_symbol:
			if (token.symbol == '(') {
				return parseParen();
			} else {
				return parseBinOp();
			}
		default: throw ParseException("Unexpected token when expecting an expression");
	}
}
