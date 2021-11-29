#pragma once

namespace Lang
{
	class ExprVisitor;

	class Expr
	{
	public:
		virtual ~Expr() = default;

		virtual void accept(ExprVisitor& visitor) const = 0;
		virtual const char* name() const = 0;
	};
}