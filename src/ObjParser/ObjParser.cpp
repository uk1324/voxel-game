//#include <ObjParser/ObjParser.hpp>
//
//std::optional<Model> Obj::ObjParser::parse(std::string_view text)
//{
//	skipWhitespace();
//
//	char c = peek();
//	advance();
//
//	switch (c)
//	{
//		case '#':
//			comment();
//			break;
//
//		case 'v':
//			break;
//
//		case 'f':
//			break;
//
//		default:
//			return std::nullopt;
//	}
//
//	return std::nullopt;
//}
//
//void Obj::ObjParser::comment()
//{
//	while ((isAtEnd() == false) && (match('\n') == false))
//		advance();
//}
//
//std::optional<Vec3> Obj::ObjParser::vector()
//{
//	skipWhitespace();
//	return Vec3();
//
//}
//
//std::optional<float>  Obj::ObjParser::number()
//{
//	//if (isDigit(peek() == false))
//
//
//	float num = peek();
//	return num;
//}
//
//bool Obj::ObjParser::isAtEnd() const
//{
//	return m_currentCharIndex >= m_text.length();
//}
//
//char Obj::ObjParser::peek() const
//{
//	return m_text[m_currentCharIndex];
//}
//
//bool Obj::ObjParser::match(char c)
//{
//	if (peek() == c)
//	{
//		advance();
//		return true;
//	}
//	return false;
//}
//
//void Obj::ObjParser::advance()
//{
//	if (isAtEnd() == false)
//		m_currentCharIndex++;
//}
//
//bool Obj::ObjParser::isWhitespace(char c)
//{
//	return (c == ' ') || (c == '\t') || (c == '\r');
//}
//
//void Obj::ObjParser::skipWhitespace()
//{
//	while (isWhitespace(peek()))
//		advance();
//}
//
//bool Obj::ObjParser::isDigit(char c)
//{
//	return (c >= '0') && (c <= '9');
//}
//
//std::optional<Model> Obj::parse(std::string_view text)
//{
//	ObjParser parser;
//	return parser.parse(text);
//}
