#include <Lang/Ast/Expr/NumberConstantExpr.hpp>
#include <Lang/Ast/ExprVisitor.hpp>
#include "NumberConstantExpr.hpp"

using namespace Lang;

NumberConstantExpr::NumberConstantExpr(Token token)
	: number(token)
{}

void NumberConstantExpr::accept(ExprVisitor& visitor) const
{
	visitor.visitNumberConstantExpr(*this);
}

const char* NumberConstantExpr::name() const
{
	return "NumberConstant";
}
