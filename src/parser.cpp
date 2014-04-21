#include "parser.hpp"
#include <iostream>

// Token buffer
Token Parser::getToken() {
	return token = lexer_.getToken();
}

DefPtr Parser::parseLine(std::string line) {
	lexer_.setLine(line + "\n");
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

	if (token.type != TokenType::symbol || token.str != ")")
		throw ParseException("Expected ')'");
	getToken(); // Eat )
	
	return expr;
}

ExprPtr Parser::parseCall(bool inCall) {
	std::string name = token.str;
	getToken(); // Eat function name

	std::vector<ExprPtr> args;
	if (!inCall) {
		while (token.type != TokenType::eof && token.type != TokenType::endl
			&& (token.type != TokenType::symbol || token.str != ")")) {
			args.push_back(std::move(parseExpr(true)));
		}
	}

	return ExprPtr(new CallExpr(name, std::move(args)));
}

ExprPtr Parser::parseExpr(bool inCall) {
	switch (token.type) {
		case TokenType::number:
			return parseNumber();
		case TokenType::symbol:
			if (token.str == "(") {
				return parseParen();
			} else {
				return parseCall(inCall);
			}
		case TokenType::identifier: {
			return parseCall(inCall);
		}
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

		if (token.type != TokenType::symbol || token.str != "=") {
			throw ParseException("Expected '=' in definition");
		}
		getToken();

		return DefPtr(new Definition(std::move(name), std::move(args), parseExpr()));
	}
	return DefPtr(new Definition(parseExpr()));
}