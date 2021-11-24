#pragma once

#include <Lang/Op.hpp>

#include <stdint.h>
#include <vector>

namespace Lang
{
	class ByteCode
	{
	public:
		void emitOp(Op op);
		void emitByte(uint8_t byte);
		void emitWord();

	public:
		std::vector<uint8_t> data;
	};
}