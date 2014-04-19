#include "parser.hpp"
#include <iostream>

int Parser::parseLine(std::string line) {
	lexer_.setLine(line);
	eat();
	ExprPtr expr = parseExpr();
	return expr->evaluate();
}

// Token buffer
Token Parser::eat() {
	return token = lexer_.getToken();
}

ExprPtr Parser::parseNumber() {
	ExprPtr result = ExprPtr(new NumberExpr(token.num));
	eat(); // Eat number
	return result;
}

ExprPtr Parser::parseParen() {
	eat(); // Eat (

	ExprPtr expr = parseExpr();

	if (token.symbol != ')')
		throw ParseException("Expected ')'");
	eat(); // Eat )
	
	return expr;
}

ExprPtr Parser::parseBinOp() {
	char op = token.symbol;
	if (op != '+' && op != '-' && op != '*' && op != '/') {
		throw ParseException("Unknown binary operator");
	}
	eat(); // Eat operator

	ExprPtr lhs = parseExpr();
	ExprPtr rhs = parseExpr();

	return ExprPtr(new BinaryExpr(op, std::move(lhs), std::move(rhs)));
}

ExprPtr Parser::parseExpr() {
	switch (token.type) {
		case TokenType::number:
			return parseNumber();
		case TokenType::symbol:
			if (token.symbol == '(') {
				return parseParen();
			} else {
				return parseBinOp();
			}
		default: throw ParseException("Unexpected token when expecting an expression");
	}
}
