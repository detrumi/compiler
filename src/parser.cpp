#include "parser.hpp"
#include <iostream>

int Parser::parseLine(std::string line) {
	lexer_.setLine(line);
	getNextToken();
	Expr *expr = parseExpr();
	return expr->evaluate();
}

// Token buffer
Token Parser::getNextToken() {
	token = lexer_.getToken();
	return token;
}


Expr *Parser::parseNumber() {
	std::cout << "parseNumber()" << std::endl;
	Expr *result = new NumberExpr(token.num);
	getNextToken(); // Eat number
	return result;
}

// (expr)
Expr *Parser::parseParen() {
	std::cout << "parseParen()" << std::endl;
	getNextToken(); // Eat (

	Expr *expr = parseExpr();

	if (token.symbol != ')')
		throw ParseException("expected ')'");
	getNextToken(); // Eat )
	
	return expr;
}

// num || (expr)
Expr *Parser::parsePrimary() {
	std::cout << "parsePrimary()" << std::endl;
	switch (token.type) {
		case TokenType::tok_number: 
			return parseNumber();
		case TokenType::tok_symbol:
			if (token.symbol == '(') {
				return parseParen();
			}
			break;
		default: break;
	}
	std::cout << token.symbol << std::endl;
	throw ParseException("unknown token when expecting an expression");
}

// (op primaryExpr)*
Expr *Parser::parseBinOpRhs(int exprPrec, Expr *lhs) {
	std::cout << "parseBinOpRhs()" << std::endl;
	while (1) {
		int tokPrec = binOpPrecedence_[token.symbol];
		if (tokPrec < exprPrec)
			return lhs;

		char binOp = token.symbol;
		getNextToken(); // Eat binOp

		Expr *rhs = parsePrimary();

		if (token.type != TokenType::tok_symbol || token.symbol == ';')
			return new BinaryExpr(binOp, lhs, rhs);

		int nextPrec = binOpPrecedence_[token.symbol];
		if (tokPrec < nextPrec) {
			rhs = parseBinOpRhs(tokPrec + 1, rhs);
		}

		lhs = new BinaryExpr(binOp, lhs, rhs);
	}
}

Expr *Parser::parseExpr() {
	std::cout << "parseExpr()" << std::endl;
	Expr *lhs = parsePrimary();
	if (token.symbol == ';')
		return lhs;
	else
		return parseBinOpRhs(0, lhs);
}
