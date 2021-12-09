#pragma once

#include <Lang/Ast/Stmt.hpp>
#include <Lang/Ast/Expr.hpp>
#include <Utils/SmartPointers.hpp>

namespace Lang
{
	class ExprStmt : public Stmt
	{
	public:
		ExprStmt(OwnPtr<Expr>&& expr, size_t start, size_t end);

		void accept(StmtVisitor& visitor) const override;
		const char* name() const override;

	public:
		OwnPtr<Expr> expr;
	};
}