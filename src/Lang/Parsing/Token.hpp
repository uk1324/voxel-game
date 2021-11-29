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
		explicit Token(TokenType type, std::string_view text);

		// TODO: Maybe make both const
		TokenType type;
		std::string_view text;
	};
}