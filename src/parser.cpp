#include "parser.hpp"
#include <iostream>

// Token buffer
Token Parser::getToken() {
	return token = lexer_.getToken();
}

DefPtr Parser::parseLine(std::string line) {
	lexer_.setLine(line);
	getToken(); // Get first token
	return parseDef();
}

ExprPtr Parser::parseNumber() {
	ExprPtr result = ExprPtr(new NumberExpr(token.num));
	getToken(); // Eat number
	return result;
}

ExprPtr Parser::parseParen() {
	getToken(); // Eat (

	ExprPtr expr = parseExpr();

	if (token.symbol != ')')
		throw ParseException("Expected ')'");
	getToken(); // Eat )
	
	return expr;
}

ExprPtr Parser::parseBinOp() {
	char op = token.symbol;
	if (op != '+' && op != '-' && op != '*' && op != '/') {
		throw ParseException("Unknown binary operator");
	}
	getToken(); // Eat operator

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
		case TokenType::identifier:
			return ExprPtr(new VarExpr(token.str));
		default: throw ParseException("Unexpected token when expecting an expression");
	}
}


DefPtr Parser::parseDef() {
	if (token.type == TokenType::identifier && token.str == "def") {
		getToken(); // Eat 'def'

		if (token.type != TokenType::identifier) {
			throw ParseException("Excepted function name after 'def'");
		}
		std::string name = token.str;
		getToken();

		std::vector<std::string> args;
		while (token.type == TokenType::identifier) {
			args.push_back(token.str);
			getToken();
		}

		if (token.type != TokenType::symbol || token.symbol != '=') {
			throw ParseException("Expected '=' in definition");
		}
		getToken();

		return DefPtr(new Definition(std::move(name), std::move(args), parseExpr()));
	}
	return DefPtr(new Definition(parseExpr()));
}