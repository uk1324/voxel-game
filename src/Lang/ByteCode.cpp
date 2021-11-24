#include <Lang/ByteCode.hpp>

void Lang::ByteCode::emitOp(Op op)
{
	data.push_back(static_cast<uint8_t>(op));
}

void Lang::ByteCode::emitByte(uint8_t byte)
{
	data.push_back(byte);
}

void Lang::ByteCode::emitWord(uint16_t word)
{

}
