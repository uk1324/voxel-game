#pragma once

#include <Lang/Ast/Stmt/ExprStmt.hpp>

namespace Lang
{
	class StmtVisitor
	{
	public:
		 virtual void visitExprStmt(const ExprStmt& expr) = 0;
	};
}