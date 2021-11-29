#pragma once

#include <Lang/ByteCode.hpp>
#include <Lang/Program.hpp>

namespace Lang
{
	class Vm
	{
	public:
		void run(const Program& program);


	private:
		size_t m_instructionPointer;
	};
}