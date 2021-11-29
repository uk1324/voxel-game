#include <Lang/Ast/Expr/BinaryExpr.hpp>
#include <Lang/Ast/ExprVisitor.hpp>

using namespace Lang;

BinaryExpr::BinaryExpr(std::unique_ptr<Expr>&& lhs, std::unique_ptr<Expr>&& rhs, Token op)
	: lhs(std::move(lhs))
	, rhs(std::move(rhs))
	, op(op)
{}

void BinaryExpr::accept(ExprVisitor& visitor) const
{
	visitor.visitBinaryExpr(*this);
}

const char* BinaryExpr::name() const
{
	return "BinaryExpr";
}
