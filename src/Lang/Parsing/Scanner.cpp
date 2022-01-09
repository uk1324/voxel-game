#include <Lang/Parsing/Scanner.hpp>
#include <Utils/Assertions.hpp>

#include <unordered_map>

using namespace Lang;

Scanner::Scanner()
	: m_currentCharIndex(0)
	, m_tokenStartIndex(0)
	, m_sourceInfo(nullptr)
{}

std::vector<Token> Scanner::parse(SourceInfo& sourceInfoToComplete)
{
	m_sourceInfo = &sourceInfoToComplete;
	m_sourceInfo->lineStartOffsets.push_back(0);
	m_currentCharIndex = 0;
	m_tokenStartIndex = 0;
	m_tokens.clear();

	while (isAtEnd() == false)
	{
		m_tokens.push_back(token());
	}

	m_tokens.push_back(Token(TokenType::Eof, "", m_currentCharIndex, m_currentCharIndex));

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
		case '=': return makeToken(TokenType::Equals);
		case '(': return makeToken(TokenType::LeftParen);
		case ')': return makeToken(TokenType::RightParen);
		case ':': return makeToken(TokenType::Colon);

		default:
			if (isDigit(c))
				return number();
			if (isAlpha(c))
				return keywordOrIdentifier();

			return errorToken("illegal character");
	}
}

Token Scanner::number()
{
	while ((isAtEnd() == false) && isDigit(peek()))
		advance();

	return makeToken(TokenType::IntNumber);
}

Token Scanner::keywordOrIdentifier()
{
	static const std::unordered_map<std::string_view, TokenType> keywords = {
		{ "print", TokenType::Print },
		{ "let", TokenType::Let },
		{ "int", TokenType::Int },
		{ "float", TokenType::Float },
	};

	while (isAlnum(peek()))
		advance();

	auto token = makeToken(TokenType::Identifier);

	auto keyword = keywords.find(token.text);

	if (keyword != keywords.end())
		token.type = keyword->second;

	return token;
}

#include <iostream>

Token Scanner::makeToken(TokenType type)
{
	Token token(type, 
		m_sourceInfo->source.substr(m_tokenStartIndex, m_currentCharIndex - m_tokenStartIndex),
		m_tokenStartIndex, m_currentCharIndex
	);

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
			case '\r':
			case '\f':
				advance();
				break;

			case '\n':
				advance();
				advanceLine();
				break;

			default:
				m_tokenStartIndex = m_currentCharIndex;
				return;
		}
	}
}

Token Scanner::errorToken(const char* message)
{
	Token token = makeToken(TokenType::Error);

	std::cout << "scanner error: " << message << '\n';

	return token;
}

char Scanner::peek()
{
	return m_sourceInfo->source[m_currentCharIndex];
}

bool Scanner::isAtEnd()
{
	// Shouldn't advance past equality. Probably could remove this.
	ASSERT(m_currentCharIndex <= m_sourceInfo->source.size());
	return m_currentCharIndex >= m_sourceInfo->source.size();
}

// TODO: Support UTF-8
void Scanner::advance()
{
	if (isAtEnd() == false)
		m_currentCharIndex++;
}

void Scanner::advanceLine()
{
	m_sourceInfo->lineStartOffsets.push_back(m_currentCharIndex);
}

bool Scanner::isDigit(char c)
{
	return (c >= '0') && (c <= '9');
}

bool Scanner::isAlpha(char c)
{
	return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'));
}

bool Scanner::isAlnum(char c)
{
	return isAlpha(c) || isDigit(c);
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
