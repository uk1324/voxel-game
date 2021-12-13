#pragma once

#include <stdint.h>

namespace Lang
{
	enum class ValueType
	{
		Int,
		Float
	};

	using Int = int64_t;
	using Float = double;

	struct Value
	{
		ValueType type;

		union
		{
			Int intNumber;
			Float floatNumber;
		} as;
	};
}