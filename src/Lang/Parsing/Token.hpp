#pragma once

#include <string_view>

namespace Lang
{
	enum class TokenType
	{
		// Numbers
		IntNumber,
		FloatNumber,

		// Operators
		Plus,

		Semicolon,

		// Keywords
		Print,

		// Special
		Error,
		Eof
	};

	struct Token
	{
		Token(TokenType type, std::string_view text, size_t start, size_t end);

		// TODO: Maybe make both const
		TokenType type;
		std::string_view text;
		size_t start;
		size_t end;
	};
}