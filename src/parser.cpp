#include "parser.hpp"

// Token buffer
Token Parser::getToken() {
	return token_ = lexer_.getToken();
}

Expr Parser::parseLine(std::string line) {
	lexer_.setLine(line + "\n");
	getToken(); // Get first token

	if (token_.type == TokenType::identifier && token_.str == "let") {
		return std::make_shared<Definition>(std::move(parseDef()));
	} else {
		return parseExpr();
	}
}

Expr Parser::parseNumber() {
	Expr result = Expr(token_.num);
	getToken(); // Eat number
	return result;
}

Expr Parser::parseParen() {
	getToken(); // Eat (

	Expr expr = parseExpr();

	if (token_.type != TokenType::symbol || token_.str != ")")
		throw ParseException("Expected ')'");
	getToken(); // Eat )
	
	return expr;
}

Expr Parser::parseCall() {
	std::string name = token_.str;
	getToken(); // Eat function name
	if (!env_.paramStack_.empty()) {
		for (int i = 0; i <= env_.lambdaDepth_; i++) {
			auto params = env_.paramStack_[env_.paramStack_.size() - i - 1];
			if (params.find(name) != params.end()) { // Argument
				return Call(std::move(name), 0);
			}
		}
	}
	// Definition
	auto def = env_.definitions_.find(name);
	if (def == env_.definitions_.end()) {
		throw CodegenException("Undefined variable '" + name + "'");
	}
	return Call(def->second);
}

Expr Parser::parseExpr(int prec) {
	Expr lhs = parsePrimary(); // Parse leftmost expression

	while (token_.type == TokenType::symbol) {
		
		auto it = opPrecedence_.find(token_.str[0]);
		if (it == opPrecedence_.end()) { // Check operator exists
			break;
		}
		
		int newPrec = it->second;
		if (newPrec < prec) { // Lower precedence operator; return lhs
			break;
		}

		std::string op = token_.str;
		getToken(); // Eat operator

		auto args = std::vector<Expr> { std::move(lhs), std::move(parseExpr(newPrec + 1)) };
		lhs = Call(std::move(op), std::move(args));
	}
	return lhs;
}

Expr Parser::parsePrimary() {
	Expr expr;
	switch (token_.type) {
		case TokenType::number:
			return parseNumber();
		case TokenType::symbol:
			if (token_.str == "(") {
				expr = parseParen();
			} else if (token_.str == "\\") {
				expr = parseLambda();
			} else {
				throw ParseException("Unexpected symbol when parsing an expression");
			}
			break;
		case TokenType::identifier:
			expr = parseCall();
			break;
		default: throw ParseException("Unexpected token when expecting an expression");
	}
	
	if (expr.type() == typeid(Call)) {
		Call &call = boost::get<Call>(expr);
		int argCount = call.expectedArgs_ - call.args_.size();
		while (argCount-- > 0 && !(token_.type == TokenType::symbol && token_.str == ")")) {
			call.args_.push_back(std::move(parseExpr(99))); // Arg binds stronger than next operators
		}
	}

	return expr;
}

Expr Parser::parseLambda() {
	getToken(); // Eat '\'
	std::vector<std::string> params;
	while (token_.type == TokenType::identifier) {
		params.push_back(token_.str);
		getToken();
	}

	if (token_.type != TokenType::symbol || token_.str != ".") {
		throw ParseException("Expected '.' in lambda");
	}
	getToken(); // Eat '.'

	env_.paramStack_.push_back(std::set<std::string>(params.begin(), params.end()));
	env_.lambdaDepth_++;
	Expr body = parseExpr();
	env_.lambdaDepth_--;
	env_.paramStack_.pop_back();

	return Call(std::make_shared<Definition>("", std::move(params), std::move(body)));
}

Definition Parser::parseDef() {
	getToken(); // Eat 'def'

	if (token_.type != TokenType::identifier) {
		throw ParseException("Excepted function name after 'def'");
	}
	std::string name = token_.str;
	getToken(); // Eat name

	std::vector<std::string> params;
	while (token_.type == TokenType::identifier) {
		params.push_back(token_.str);
		getToken();
	}

	if (token_.type != TokenType::symbol || token_.str != "=") {
		throw ParseException("Expected '=' in definition");
	}
	getToken(); // Eat '='

	env_.paramStack_.push_back(std::set<std::string>(params.begin(), params.end()));
	int lambdaDepth = env_.lambdaDepth_;
	env_.lambdaDepth_ = 0;
	Expr body = parseExpr();
	env_.lambdaDepth_ = lambdaDepth;
	env_.paramStack_.pop_back();

	return Definition(std::move(name), std::move(params), std::move(body));;
}