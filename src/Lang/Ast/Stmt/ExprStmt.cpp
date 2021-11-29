#include <Lang/Ast/Stmt/ExprStmt.hpp>
#include <Lang/Ast/StmtVisitor.hpp>

using namespace Lang;

ExprStmt::ExprStmt(std::unique_ptr<Expr>&& expr)
	: expr(std::move(expr))
{}

void ExprStmt::accept(StmtVisitor& visitor) const
{
	visitor.visitExprStmt(*this);
}

const char* ExprStmt::name() const
{
	return "ExprStmt";
}