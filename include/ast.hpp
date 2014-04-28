#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

class Expr;
class Environment;

using ExprPtr = std::unique_ptr<Expr>;

class CodegenException : public std::runtime_error {
public:
	CodegenException(std::string msg) : std::runtime_error(msg) {}
};

class Expr {
public:
	virtual ~Expr() {}
	virtual int evaluate(Environment &) = 0;
};

class NumberExpr : public Expr {
	int val_;
public:
	NumberExpr(int val) : val_(val) {}
	int evaluate(Environment &) { return val_; }
};

class CallExpr : public Expr {
	std::string name_;
	std::vector<ExprPtr> args_;
public:
	CallExpr(std::string name, std::vector<ExprPtr> args)
		: name_(name), args_(std::move(args)) {}

	int evaluate(Environment &env);
};

#endif