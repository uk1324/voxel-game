#pragma once

#include <Lang/Ast/Stmt.hpp>
#include <Lang/Ast/Expr.hpp>

#include <memory>

namespace Lang
{
	class ExprStmt : public Stmt
	{
	public:
		ExprStmt(std::unique_ptr<Expr>&& expr);

		void accept(StmtVisitor& visitor) const override;
		const char* name() const override;

	public:
		std::unique_ptr<Expr> expr;
	};
}