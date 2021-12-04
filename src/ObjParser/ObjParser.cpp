#include <ObjParser/ObjParser.hpp>

using namespace Obj;

std::optional<Model> ObjParser::parse(std::string_view text)
{
	while (isAtEnd() == false)
	{
		if (parseLine() == false)
		{
			return std::nullopt;
		}
	}

	return std::nullopt;
}

bool ObjParser::parseLine()
{
	skipWhitespace();

	char c = peek();
	advance();

	switch (c)
	{
		case '#':
			comment();
			break;

		case 'v':
			break;

		case 'f':
			break;

		default:
			return false;
	}

	skipWhitespace();

	if (match('\n') == false)
		return false;

	return true;
}

void ObjParser::comment()
{
	while ((isAtEnd() == false) && (match('\n') == false))
		advance();
}

std::optional<Vec3> ObjParser::vector()
{
	skipWhitespace();
	return Vec3();

}

std::optional<float>  ObjParser::number()
{
	//if (isDigit(peek() == false))


	float num = peek();
	return num;
}

bool ObjParser::isAtEnd() const
{
	return m_currentCharIndex >= m_text.length();
}

char ObjParser::peek() const
{
	return m_text[m_currentCharIndex];
}

bool ObjParser::match(char c)
{
	if (peek() == c)
	{
		advance();
		return true;
	}
	return false;
}

void ObjParser::advance()
{
	if (isAtEnd() == false)
		m_currentCharIndex++;
}

bool ObjParser::isWhitespace(char c)
{
	return (c == ' ') || (c == '\t') || (c == '\r');
}

void ObjParser::skipWhitespace()
{
	while (isWhitespace(peek()))
		advance();
}

bool ObjParser::isDigit(char c)
{
	return (c >= '0') && (c <= '9');
}

std::optional<Model> Obj::parse(std::string_view text)
{
	ObjParser parser;
	return parser.parse(text);
}
