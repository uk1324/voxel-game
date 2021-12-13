#include <Lang/Ast/Stmt/VariableDeclarationStmt.hpp>
#include <Lang/Ast/StmtVisitor.hpp>

using namespace Lang;

VariableDeclarationStmt::VariableDeclarationStmt(size_t start, size_t end)
	: Stmt(start, end)
{}

void VariableDeclarationStmt::accept(StmtVisitor& visitor) const
{
	visitor.visitVariableDeclarationStmt(*this);
}

StmtType VariableDeclarationStmt::type() const
{
	return StmtType::VariableDeclaration;
}
