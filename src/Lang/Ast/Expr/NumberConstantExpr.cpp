#include <Lang/Ast/Expr/NumberConstantExpr.hpp>
#include <Lang/Ast/ExprVisitor.hpp>
#include "NumberConstantExpr.hpp"

using namespace Lang;

NumberConstantExpr::NumberConstantExpr(Token token, size_t start, size_t end)
	: Expr(start, end)
	, number(token)
{}

void NumberConstantExpr::accept(ExprVisitor& visitor) const
{
	visitor.visitNumberConstantExpr(*this);
}

const char* NumberConstantExpr::name() const
{
	return "NumberConstant";
}
