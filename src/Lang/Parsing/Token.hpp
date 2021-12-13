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
		Equals,

		// Symbols
		Semicolon,
		LeftParen,
		RightParen,

		// Keywords
		Print,
		Let,
		Int,
		Float,

		// Other
		Identifier,

		// Special
		Error,
		Eof
	};

	struct Token
	{
		Token(TokenType type, std::string_view text, size_t start, size_t end);

		// TODO: Maybe make both const
		TokenType type;
		// This stores basically the same information twice text and start and end in source beacuse I don't want
		// to use source information in the parser outside of error handling but this like IntConstantExpr need to parse the number.
		std::string_view text;
		size_t start;
		size_t end;
	};
}