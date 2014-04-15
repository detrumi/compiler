#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <vector>

class CodegenException : public std::runtime_error {
public:
	CodegenException(std::string msg) : std::runtime_error(msg) {}
};

class Expr {
public:
	virtual ~Expr() {}
	virtual int evaluate() = 0;
};

class NumberExpr : public Expr {
	int val_;
public:
	NumberExpr(int val) : val_(val) {}
	int evaluate() { return val_; }
};

// Binary operator
class BinaryExpr : public Expr {
	char op_;
	Expr *lhs_, *rhs_;
public:
	BinaryExpr(char op, Expr *lhs, Expr *rhs)
		: op_(op), lhs_(lhs), rhs_(rhs) {}

	int evaluate() {
		switch (op_) {
			case '+': return lhs_->evaluate() + rhs_->evaluate();
			case '-': return lhs_->evaluate() - rhs_->evaluate();
			case '*': return lhs_->evaluate() * rhs_->evaluate();
			case '/': return lhs_->evaluate() / rhs_->evaluate();
			default: throw CodegenException("Unknown binary operator");
		}
	}
};

#endif