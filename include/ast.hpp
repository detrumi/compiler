#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

class Expr;
typedef std::unique_ptr<Expr> ExprPtr;

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

class BinaryExpr : public Expr {
	char op_;
	ExprPtr lhs_, rhs_;
public:
	BinaryExpr(char op, ExprPtr lhs, ExprPtr rhs)
		: op_(op), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

	int evaluate() {
		switch (op_) {
			case '+': return lhs_->evaluate() + rhs_->evaluate();
			case '-': return lhs_->evaluate() - rhs_->evaluate();
			case '*': return lhs_->evaluate() * rhs_->evaluate();
			case '/': {
				int rhsVal = rhs_->evaluate();
				if (rhsVal == 0)
					throw CodegenException("Divide by 0 error");
				return lhs_->evaluate() / rhsVal;
			}
			default: throw CodegenException("Unknown binary operator");
		}
	}
};

#endif