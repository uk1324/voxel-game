#include <Lang/Parsing/Token.hpp>

Lang::Token::Token(TokenType type, std::string_view text)
	: type(type)
	, text(text)
{}