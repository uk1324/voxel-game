#pragma once

#include <Lang/ByteCode.hpp>

namespace Lang
{
	void disassembleByteCode(const ByteCode& code);

	class Disassembler
	{
		Disassembler(const ByteCode& code);
	};
}