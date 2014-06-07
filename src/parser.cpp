#include <iostream>

#include "parser.hpp"
#include "environment.hpp"

// Token buffer
Token Parser::getToken() {
	return token = lexer_.getToken();
}

void Parser::parseLine(std::string line) {
	lexer_.setLine(line + "\n");
	getToken(); // Get first token

	if (token.type == TokenType::identifier && token.str == "def") {
		Definition def = parseDef();
		env_.addDefinition(std::move(def));
	} else if (token.type != TokenType::endl && token.type != TokenType::eof) { // Top-level expression, return definition without name
		std::cout << parseExpr()->evaluate(env_) << std::endl;
	}
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

ExprPtr Parser::parseCall() {
	std::string name = token.str;
	getToken(); // Eat function name

	if (paramStack_.size() > 0) {
		auto params = paramStack_.top();
		if (params.find(name) != params.end()) { // Call parameter
			return ExprPtr(new CallExpr(name, 0));
		}
	}
	int paramCount = env_.getDefinition(name).paramCount();
	return ExprPtr(new CallExpr(name, paramCount));
}

ExprPtr Parser::parseExpr(int prec) {
	ExprPtr lhs = parsePrimary(); // Parse leftmost expression

	while (token.type == TokenType::symbol) {
		
		auto it = opPrecedence_.find(token.str[0]);
		if (it == opPrecedence_.end()) { // Check operator exists
			break;
		}
		
		int newPrec = it->second;
		if (newPrec < prec) { // Lower precedence operator; return lhs
			break;
		}

		std::string op = token.str;
		getToken(); // Eat operator

		std::vector<ExprPtr> args;
		args.push_back(std::move(lhs));
		args.push_back(std::move(parseExpr(newPrec + 1)));
		lhs = ExprPtr(new CallExpr(op, std::move(args)));
	}
	return std::move(lhs);
}

ExprPtr Parser::parsePrimary() {
	ExprPtr expr;
	switch (token.type) {
		case TokenType::number:
			return parseNumber();
		case TokenType::symbol:
			if (token.str == "(") {
				expr = parseParen();
			} else if (token.str == "\\") {
				expr = parseLambda();
			} else {
				expr = parseCall();
			}
			break;
		case TokenType::identifier:
			expr = parseCall();
			break;
		default: throw ParseException("Unexpected token when expecting an expression");
	}
	
	while (!(token.type == TokenType::symbol && token.str == ")") && expr->expectArg()) {
		expr->addArg(std::move(parseExpr(99))); // Arg binds stronger than next operators
	}
	return std::move(expr);
}

ExprPtr Parser::parseLambda() {
	getToken(); // Eat '\'
	std::vector<std::string> params;
	while (token.type == TokenType::identifier) {
		params.push_back(token.str);
		getToken();
	}

	if (token.type != TokenType::symbol || token.str != ".") {
		throw ParseException("Expected '.' in lambda");
	}
	getToken(); // Eat '.'

	// TODO retain old parameters
	paramStack_.push(std::set<std::string>(params.begin(), params.end()));
	ExprPtr body = parseExpr();
	paramStack_.pop();

	return ExprPtr(new LambdaExpr(std::move(params), std::move(body)));
}

Definition Parser::parseDef() {
	getToken(); // Eat 'def'

	if (token.type != TokenType::identifier) {
		throw ParseException("Excepted function name after 'def'");
	}
	std::string name = token.str;
	getToken(); // Eat name

	std::vector<std::string> params;
	while (token.type == TokenType::identifier) {
		params.push_back(token.str);
		getToken();
	}

	if (token.type != TokenType::symbol || token.str != "=") {
		throw ParseException("Expected '=' in definition");
	}
	getToken(); // Eat '='

	paramStack_.push(std::set<std::string>(params.begin(), params.end()));;
	Definition def =  Definition(std::move(name), std::move(params), parseExpr());
	paramStack_.pop();
	return def;
}