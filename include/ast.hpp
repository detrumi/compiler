#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <vector>

class CodegenException : public std::runtime_error {
public:
	CodegenException(std::string msg) : std::runtime_error(msg) {}
};

class Function;

class Expr {
public:
	virtual ~Expr() {}
};

class NumberExpr : public Expr {
	double val_;
public:
	NumberExpr(double val) : val_(val) {}
};

// Binary operator
class BinaryExpr : public Expr {
	char op_;
	Expr *lhs_, *rhs_;
public:
	BinaryExpr(char op, Expr *lhs, Expr *rhs)
		: op_(op), lhs_(lhs), rhs_(rhs) {}
};

#endif