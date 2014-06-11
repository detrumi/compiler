#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <boost/variant.hpp>

struct Call;
struct Definition;

using DefPtr = std::shared_ptr<Definition>;

using Expr = boost::variant< int
							, boost::recursive_wrapper<Call>
						    , boost::recursive_wrapper<DefPtr> >;

class CodegenException : public std::runtime_error {
public:
	CodegenException(std::string msg) : std::runtime_error(msg) {}
};

struct Definition { // User-defined definition or lambda
	Definition(std::string name, std::vector<std::string> params, Expr body)
		: name_(std::move(name)), params_(std::move(params)), body_(std::move(body)) {}

	std::string name_;
	std::vector<std::string> params_;
	Expr body_;
};

struct Call {
	Call(std::string name, int expectedArgs) // Argument or builtin
		: target_(std::move(name)), expectedArgs_(expectedArgs) {}
	Call(DefPtr def) // Def or lambda
		: target_(def), expectedArgs_(def->params_.size()) {}
	Call(std::string op, std::vector<Expr> args) // Builtin operators
		: target_(std::move(op)), args_(std::move(args)), expectedArgs_(2) {}

	boost::variant<DefPtr, std::string> target_; // Def or argument/builtin
	std::vector<Expr> args_;
	int expectedArgs_;
};

#endif