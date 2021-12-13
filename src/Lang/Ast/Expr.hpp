#pragma once

#include <Lang/TypeChecking/DataType.hpp>
#include <stddef.h>

namespace Lang
{
	enum class ExprType
	{
		IntConstant,
		FloatConstant,
		Binary
	};

	const char* exprTypeToString(ExprType type);

	class ExprVisitor;

	class Expr
	{
	public:
		Expr(size_t start, size_t end);
		virtual ~Expr() = default;

		virtual void accept(ExprVisitor& visitor) const = 0;
		virtual ExprType type() const = 0;

	public:
		size_t start;
		size_t length;

		DataType dataType;
	};
}