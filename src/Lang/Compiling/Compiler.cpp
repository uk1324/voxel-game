#include <Lang/Compiling/Compiler.hpp>

using namespace Lang;

Compiler::Compiler()
{
}

#include <iostream>

void Compiler::visitBinaryExpr(const BinaryExpr& expr)
{
	std::cout << "Binay expr";

	expr.lhs->accept(*this);

	expr.rhs->accept(*this);
}

void Compiler::visitNumberConstantExpr(const NumberConstantExpr& expr)
{
	std::cout << "Number constant expr";
}

Program Compiler::compile(const Expr& expr)
{
	m_program = Program();

	expr.accept(*this);

	return std::move(m_program);
}
