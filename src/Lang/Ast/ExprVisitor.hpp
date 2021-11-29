#pragma once

#include <Lang/Ast/Expr/BinaryExpr.hpp>
#include <Lang/Ast/Expr/NumberConstantExpr.hpp>

namespace Lang
{
	class ExprVisitor
	{
	public:
		virtual void visitBinaryExpr(const BinaryExpr& expr) = 0;
		virtual void visitNumberConstantExpr(const NumberConstantExpr& expr) = 0;
	};
}