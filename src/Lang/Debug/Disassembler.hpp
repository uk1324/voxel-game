#pragma once

#include <Lang/ByteCode.hpp>

#include <string_view>

namespace Lang
{
	class Disassembler
	{
		friend void disassembleByteCode(const ByteCode& code);

	private:
		Disassembler(const ByteCode& code);

		void disassemble();
		void disassembleInstruction(Op op);

		void simpleInstruction(std::string_view name);

	private:
		const ByteCode& m_code;
		size_t m_index;
	};
}