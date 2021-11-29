#include <Lang/Parsing/Scanner.hpp>
#include <Utils/Assertions.hpp>

using namespace Lang;

Scanner::Scanner()
	: m_currentCharIndex(0)
	, m_tokenStartIndex(0)
{}

std::vector<Token> Scanner::parse(std::string_view source)
{
	m_source = source;
	m_currentCharIndex = 0;
	m_tokenStartIndex = 0;
	m_tokens.clear();

	while (isAtEnd() == false)
	{
		m_tokens.push_back(token());
	}

	m_tokens.push_back(Token(TokenType::Eof, ""));

	return m_tokens;
}

Token Scanner::token()
{
	skipWhitespace();

	char c = peek();
	advance();

	switch (c)
	{
		case '+': return makeToken(TokenType::Plus);
		case ';': return makeToken(TokenType::Semicolon);

		default:
			if (isDigit(c))
				return number();
	}
}

Token Scanner::number()
{
	while ((isAtEnd() == false) && isDigit(peek()))
		advance();

	return makeToken(TokenType::IntNumber);
}

#include <iostream>

Token Scanner::makeToken(TokenType type)
{
	Token token(type, m_source.substr(m_tokenStartIndex, m_currentCharIndex - m_tokenStartIndex));
	m_tokenStartIndex = m_currentCharIndex;
	return token;
}

void Scanner::skipWhitespace()
{
	while (isAtEnd() == false)
	{
		char c = peek();

		switch (c)
		{
			case ' ':
			case '\t':
			case '\n':
			case '\r':
			case '\f':
				advance();
				break;

			default:
				m_tokenStartIndex = m_currentCharIndex;
				return;
		}
	}
}

char Scanner::peek()
{
	return m_source[m_currentCharIndex];
}

bool Scanner::isAtEnd()
{
	// Shouldn't advance past equality. Probably could remove this.
	ASSERT(m_currentCharIndex <= m_source.size());
	return m_currentCharIndex >= m_source.size();
}

// TODO: Support UTF-8
void Scanner::advance()
{
	if (isAtEnd() == false)
		m_currentCharIndex++;
}

bool Scanner::isDigit(char c)
{
	return (c >= '0') && (c <= '9');
}

bool Scanner::match(char c)
{
	if (peek() == c)
	{
		advance();
		return true;
	}
	return false;
}
