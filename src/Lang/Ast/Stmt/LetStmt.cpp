#include <Lang/Ast/Stmt/LetStmt.hpp>
#include <Lang/Ast/StmtVisitor.hpp>

using namespace Lang;

LetStmt::LetStmt(size_t start, size_t end)
	: Stmt(start, end)
{}

void LetStmt::accept(StmtVisitor& visitor) const
{
	visitor.visitLetStmt(*this);
}

StmtType LetStmt::type() const
{
	return StmtType::VariableDeclaration;
}
