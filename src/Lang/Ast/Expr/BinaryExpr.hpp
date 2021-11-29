#pragma once

#include <Lang/Ast/Expr.hpp>
#include <Lang/Parsing/Token.hpp>

#include <memory>

namespace Lang
{
	class BinaryExpr : public Expr
	{
	public:
		BinaryExpr(std::unique_ptr<Expr>&& lhs, std::unique_ptr<Expr>&& rhs, Token op);

		void accept(ExprVisitor& visitor) const override;
		const char* name() const override;

	public:
		std::unique_ptr<Expr> lhs;
		std::unique_ptr<Expr> rhs;
		Token op;
	};
}