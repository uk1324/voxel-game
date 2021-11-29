#pragma once

#include <Lang/Ast/Expr.hpp>
#include <Lang/Parsing/Token.hpp>

namespace Lang
{
	class NumberConstantExpr : public Expr
	{
	public:
		NumberConstantExpr(Token token);

		void accept(ExprVisitor& visitor) const override;
		const char* name() const override;

	public:
		Token number;
	};
}