#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <stdexcept>
#include <memory>

class Expr;
class Environment;

typedef std::unique_ptr<Expr> ExprPtr;

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

class BinaryExpr : public Expr {
	char op_;
	ExprPtr lhs_, rhs_;
public:
	BinaryExpr(char op, ExprPtr lhs, ExprPtr rhs)
		: op_(op), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

	int evaluate(Environment &env);
};

class VarExpr : public Expr {
	std::string name_;
public:
	VarExpr(std::string name) : name_(std::move(name)) {}

	int evaluate(Environment &env);
};

#endif