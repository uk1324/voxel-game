#pragma once

#include <Lang/Parsing/Token.hpp>

#include <vector>
#include <string_view>

// Maybe make every function private and only expose a friend function like std::vector<Token> parseTokens(std::string_view source);
// The problem is that it would need to construct a new object for each parse.
// I could return a std::move(m_tokens);

namespace Lang
{
	class Scanner
	{
	public:
		Scanner();

		std::vector<Token> parse(std::string_view source);

	private:
		Token token();

		Token number();

		Token makeToken(TokenType type);

		void skipWhitespace();

		char peek();
		bool isAtEnd();
		void advance();

		bool isDigit(char c);
		bool match(char c);

	private:
		std::vector<Token> m_tokens;

		// It could be faster to just increment a pointer.
		std::string_view m_source;

		size_t m_currentCharIndex;
		size_t m_tokenStartIndex;
	};
}