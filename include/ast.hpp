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
	int argsExpected_;
public:
	Expr() : argsExpected_(0) {}
	Expr(int argsExpected) : argsExpected_(argsExpected) {}

	virtual ~Expr() {}
	virtual int evaluate(Environment &) = 0;
	virtual void addArg(ExprPtr) { throw CodegenException("Can't add argument"); }

	bool expectArg();
};

class NumberExpr : public Expr {
	int val_;
public:
	NumberExpr(int val) : Expr(), val_(val) {}
	int evaluate(Environment &) { return val_; }
};

class CallExpr : public Expr {
	std::string name_;
	std::vector<ExprPtr> args_;
public:
	CallExpr(std::string name, int paramCount)
		: Expr(paramCount), name_(name) {}
	CallExpr(std::string name, std::vector<ExprPtr> args)
		: Expr(), name_(name), args_(std::move(args)) {}

	int evaluate(Environment &env);
	void addArg(ExprPtr);
};

class LambdaExpr : public Expr {
	std::vector<std::string> params_;
	ExprPtr body_;
	std::vector<ExprPtr> args_;
public:
	LambdaExpr(std::vector<std::string> params, ExprPtr body)
		: Expr(params.size()), params_(std::move(params)), body_(std::move(body)) {}

	int evaluate(Environment &);
	void addArg(ExprPtr);
};

#endif