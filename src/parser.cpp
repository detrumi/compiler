#include "parser.hpp"

// Token buffer
Token Parser::getNextToken() {
	return token = lexer_.getToken();
}


Expr *Parser::parseNumber() {
	Expr *result = new NumberExpr(token.num);
	getNextToken(); // Eat number
	return result;
}

Expr *Parser::parseParen() {
	getNextToken(); // Eat (

	Expr *expr = parseExpr();

	if (token.symbol != ')')
		throw ParseException("expected ')'");
	getNextToken(); // Eat )
	
	return expr;
}

Expr *Parser::parsePrimary() {
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
	throw ParseException("unknown token when expecting an expression");
}

// (op primaryExpr)*
Expr *Parser::parseBinOpRhs(int exprPrec, Expr *lhs) {
	while (1) {
		int tokPrec = binOpPrecedence_[token.symbol];
		if (tokPrec < exprPrec)
			return lhs;

		int binOp = token.symbol;
		getNextToken(); // Eat binOp

		Expr *rhs = parsePrimary();

		int nextPrec = binOpPrecedence_[token.symbol];
		if (tokPrec < nextPrec) {
			rhs = parseBinOpRhs(tokPrec + 1, rhs);
		}

		lhs = new BinaryExpr(binOp, lhs, rhs);
	}
}

Expr *Parser::parseExpr() {
	Expr *lhs = parsePrimary();
	if (!lhs) throw ParseException("Invalid expression");
	return parseBinOpRhs(0, lhs);
}
