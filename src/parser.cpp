#include <iostream>

#include "parser.hpp"
#include "environment.hpp"
#include "evaluator.hpp"

// Token buffer
Token Parser::getToken() {
	return token = lexer_.getToken();
}

void Parser::parseLine(std::string line) {
	lexer_.setLine(line + "\n");
	Evaluator evaluator(env_);
	getToken(); // Get first token

	if (token.type == TokenType::identifier && token.str == "def") {
		Definition def = parseDef();
		env_.addDefinition(std::move(def));
	} else if (token.type != TokenType::endl && token.type != TokenType::eof) { // Top-level expression, return definition without name
		Expr expr = parseExpr();
		std::cout << evaluator.eval(expr) << std::endl;
	}
}

Expr Parser::parseNumber() {
	Expr result = Expr(token.num);
	getToken(); // Eat number
	return result;
}

Expr Parser::parseParen() {
	getToken(); // Eat (

	Expr expr = parseExpr();

	if (token.type != TokenType::symbol || token.str != ")")
		throw ParseException("Expected ')'");
	getToken(); // Eat )
	
	return expr;
}

Expr Parser::parseCall() {
	std::string name = token.str;
	getToken(); // Eat function name
	return Call(std::move(name));
}

Expr Parser::parseExpr(int prec) {
	Expr lhs = parsePrimary(); // Parse leftmost expression

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

		auto args = std::vector<Expr> { std::move(lhs), std::move(parseExpr(newPrec + 1)) };
		lhs = Call(op, std::move(args));
	}
	return std::move(lhs);
}

Expr Parser::parsePrimary() {
	Expr expr;
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
	
	if (expr.type() == typeid(Call)) {
		Call &call = boost::get<Call>(expr);
		if (paramStack_.empty() || paramStack_.top().find(call.name_) == paramStack_.top().end()) { // Don't expect args on parameter call
			if (opPrecedence_.find(call.name_[0]) != opPrecedence_.end()) { // Builtin operator
				parseArgs(2, call.args_);
			} else { // User-defined function call
				Definition &def = boost::get<Definition>(env_.getDefinition(call.name_));
				parseArgs(def.params_.size(), call.args_);
			}
		}
	}

	return std::move(expr);
}

void Parser::parseArgs(int argCount, std::vector<Expr> &target) {
	argCount -= target.size();
	while (argCount-- > 0 && !(token.type == TokenType::symbol && token.str == ")")) {
		target.push_back(std::move(parseExpr(99))); // Arg binds stronger than next operators
	}
}

Expr Parser::parseLambda() {
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
	Expr body = parseExpr();
	paramStack_.pop();

	auto lambdaName = env_.addLambda(std::move(params), std::move(body));
	return Call(std::move(lambdaName));
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

	paramStack_.push(std::set<std::string>(params.begin(), params.end()));
	Expr body = parseExpr();
	paramStack_.pop();

	return Definition(std::move(name), std::move(params), std::move(body));;
}