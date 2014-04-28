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

	bool isParam = false;
	if (paramStack_.size() > 0) {
		auto params = paramStack_.top();
		isParam = params.find(name) != params.end();
	}

	std::vector<ExprPtr> args;
	if (!isParam) {
		int paramCount = env_.getDefinition(name).paramCount();
		// TODO give Definition& to a CallExpr instead of just its name

		while (paramCount-- > 0 && token.type != TokenType::eof && token.type != TokenType::endl
			&& (token.type != TokenType::symbol || token.str != ")")) {
			args.push_back(std::move(parsePrimary()));
		}
	}

	return ExprPtr(new CallExpr(name, std::move(args)));
}

ExprPtr Parser::parseExpr(int prec) {
	ExprPtr lhs = parsePrimary();

	decltype(opPrecedence_.begin()) it;
	while (token.type == TokenType::symbol && 
		(it = opPrecedence_.find(token.str[0])) != opPrecedence_.end()) {
		int newPrec = it->second;
		if (newPrec < prec) {
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
	switch (token.type) {
		case TokenType::number:
			return parseNumber();
		case TokenType::symbol:
			if (token.str == "(") {
				return parseParen();
			} else {
				return parseCall();
			}
		case TokenType::identifier: {
			return parseCall();
		}
		default: throw ParseException("Unexpected token when expecting an expression");
	}
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
	getToken(); // Eat =

	paramStack_.push(std::set<std::string>(params.begin(), params.end()));;
	Definition def =  Definition(std::move(name), std::move(params), parseExpr());
	paramStack_.pop();
	return def;
}