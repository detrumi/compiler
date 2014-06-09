#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <boost/variant.hpp>

class Environment;

class CodegenException : public std::runtime_error {
public:
	CodegenException(std::string msg) : std::runtime_error(msg) {}
};

struct Call; struct Lambda; struct Definition;
using Expr = boost::variant< int
							, Call
						    , boost::recursive_wrapper<Lambda>
						    , boost::recursive_wrapper<Definition> >;

struct Call {
	Call(std::string name)
		: name_(std::move(name)) {}
	Call(std::string name, std::vector<Expr> args)
		: name_(std::move(name)), args_(std::move(args)) {}

	std::string name_;
	std::vector<Expr> args_;
};

struct Lambda {
	Lambda(std::vector<std::string> params, Expr body)
		: params_(std::move(params)), body_(std::move(body)) {}

	std::vector<std::string> params_;
	Expr body_;
	std::vector<Expr> args_;
};

struct Definition {
	Definition(std::string name, std::vector<std::string> params, Expr body)
		: name_(std::move(name)), params_(std::move(params)), body_(std::move(body)) {}

	std::string name_;
	std::vector<std::string> params_;
	Expr body_;
};

#endif