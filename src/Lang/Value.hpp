#pragma once

#include <stdint.h>

namespace Lang
{
	enum class ValueType
	{
		Int,
		Float
	};

	using Integer = int64_t;
	using Float = double;

	struct Value
	{
		ValueType type;

		union
		{
			Integer intNumber;
			Float floatNumber;
		} as;
	};
}