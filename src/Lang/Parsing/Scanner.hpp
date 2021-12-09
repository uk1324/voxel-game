#pragma once

#include <Lang/Parsing/Token.hpp>
#include <Lang/Parsing/SourceInfo.hpp>

// Maybe make every function private and only expose a friend function like std::vector<Token> parseTokens(std::string_view source);
// The problem is that it would need to construct a new object for each parse.
// I could return a std::move(m_tokens);

namespace Lang
{
	class Scanner
	{
	public:
		Scanner();

		std::vector<Token> parse(SourceInfo& sourceInfoToComplete);

	private:
		Token token();

		Token number();

		Token makeToken(TokenType type);

		void skipWhitespace();

		char peek();
		bool isAtEnd();
		void advance();
		void advanceLine();

		bool isDigit(char c);
		bool match(char c);

	private:
		std::vector<Token> m_tokens;

		// It could be faster to just increment a pointer.
		SourceInfo* m_sourceInfo;


		size_t m_currentCharIndex;
		size_t m_tokenStartIndex;
	};
}