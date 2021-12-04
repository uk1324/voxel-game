//#pragma once
//
//#include <Engine/Graphics/Model.hpp>
//
//#include <string_view>
//#include <optional>
//
//namespace Obj
//{
//	std::optional<Model> parse(std::string_view text);
//
//	class ObjParser
//	{
//	public:
//		std::optional<Model> parse(std::string_view text);
//
//	private:
//		void comment();
//		std::optional<Vec3> vector();
//		std::optional<float> number();
//
//		bool isAtEnd() const;
//		char peek() const;
//		bool match(char c);
//		void advance();
//
//		static bool isWhitespace(char c);
//		void skipWhitespace();
//
//		static bool isDigit(char c);
//
//	private:
//		std::string_view m_text;
//		size_t m_currentCharIndex;
//	};
//}