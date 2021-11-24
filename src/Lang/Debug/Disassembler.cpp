#include <Lang/Debug/Disassembler.hpp>

void Lang::disassembleByteCode(const ByteCode& code)
{
	size_t i = 0;

	while (i < code.data.size())
	{
		switch (static_cast<Op>(code.data[i]))
		{
			case Op::Add:

				break;
		}
	}
}
