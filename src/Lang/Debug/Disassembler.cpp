#include "Disassembler.hpp"
#include "Disassembler.hpp"
#include <Lang/Debug/Disassembler.hpp>

#include <iostream>
#include <iomanip>

void Lang::disassembleByteCode(const ByteCode& code)
{
	Disassembler disassembler(code);
	disassembler.disassemble();
}

Lang::Disassembler::Disassembler(const ByteCode& code)
	: m_code(code)
	, m_index(0)
{}

void Lang::Disassembler::disassemble()
{
	while (m_index < m_code.data.size())
	{
		std::cout << std::setw(6) << m_index << " | ";

		disassembleInstruction(static_cast<Op>(m_code.data[m_index]));

		std::cout << '\n';
	}
}

void Lang::Disassembler::disassembleInstruction(Op op)
{
	switch (op)
	{
		case Lang::Op::Add:	   simpleInstruction("add"); break;
		case Lang::Op::Return: simpleInstruction("return"); break;

		default:
			std::cout << "invalid op code";
			break;
	}
}

void Lang::Disassembler::simpleInstruction(std::string_view name)
{
	std::cout << name;
	m_index++;
}
