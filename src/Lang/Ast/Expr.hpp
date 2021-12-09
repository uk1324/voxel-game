#pragma once

#include <stddef.h>

namespace Lang
{
	class ExprVisitor;

	class Expr
	{
	public:
		Expr(size_t start, size_t end);
		virtual ~Expr() = default;

		virtual void accept(ExprVisitor& visitor) const = 0;
		virtual const char* name() const = 0;

	public:
		size_t start;
		size_t end;
	};
}