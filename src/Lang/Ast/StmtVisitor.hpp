#pragma once

#include <Lang/Ast/Stmt/ExprStmt.hpp>
#include <Lang/Ast/Stmt/PrintStmt.hpp>
#include <Lang/Ast/Stmt/LetStmt.hpp>

namespace Lang
{
	class StmtVisitor
	{
	public:
		 virtual void visitExprStmt(const ExprStmt& expr) = 0;
		 virtual void visitPrintStmt(const PrintStmt& expr) = 0;
		 virtual void visitLetStmt(const LetStmt& expr) = 0;
	};

	class NonConstStmtVisitor
	{
	public:
		virtual void visitExprStmt(ExprStmt& expr) = 0;
		virtual void visitPrintStmt(PrintStmt& expr) = 0;
		virtual void visitLetStmt(LetStmt& expr) = 0;
	};
}