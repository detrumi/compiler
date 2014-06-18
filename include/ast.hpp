#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <boost/variant.hpp>

struct Parameter;
struct Call;
struct Lambda;
struct Definition;

using DefPtr = std::shared_ptr<Definition>;

using Expr = boost::variant< int
							, Parameter
							, boost::recursive_wrapper<Call>
							, boost::recursive_wrapper<Lambda>
						    , boost::recursive_wrapper<DefPtr> >;

class CodegenException : public std::runtime_error {
public:
	CodegenException(std::string msg) : std::runtime_error(msg) {}
};

struct Parameter { // Parameter call
	Parameter(std::string name)
		: name_(std::move(name)) {}

	std::string name_;
};

struct Lambda {
	Lambda(std::vector<std::string> params, Expr body)
		: params_(std::move(params)), body_(std::move(body)) {}

	std::vector<std::string> params_;
	Expr body_;
};

struct Definition { // User-defined definition
	Definition(std::string name, Lambda body)
		: name_(std::move(name)), body_(std::move(body)) {}

	std::string name_;
	Lambda body_;
};

struct Call {
	using TargetType = boost::variant<DefPtr, Lambda, std::string>;

	Call(TargetType target, int expectedArgs)
		: target_(std::move(target))
	{
		args_.reserve(expectedArgs);
	}

	Call(std::string op, std::vector<Expr> args) // Builtin operators
		: target_(std::move(op)), args_(std::move(args)) {}

	TargetType target_; // def, lambda or builtin
	std::vector<Expr> args_;
};

#endif