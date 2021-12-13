#pragma once

#include <Lang/Program.hpp>
#include <Lang/Ast/ExprVisitor.hpp>

namespace Lang
{
	class Compiler : public ExprVisitor
	{
	public:
		Compiler();

		void visitBinaryExpr(const BinaryExpr& expr) override;
		void visitIntConstantExpr(const IntConstantExpr& expr) override;
		void visitFloatConstantExpr(const FloatConstantExpr& expr) override;

		Program compile(const Expr& expr);

	private:
		Program m_program;
	};
}