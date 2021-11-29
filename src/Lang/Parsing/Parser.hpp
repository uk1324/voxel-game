#pragma once

#include <Lang/Ast/Expr.hpp>
#include <Lang/Ast/Stmt.hpp>
#include <Lang/Parsing/Token.hpp>

#include <vector>
#include <memory>

namespace Lang
{
	class Parser
	{
	public:
		Parser();

		// Could use move here.
		std::vector<std::unique_ptr<Stmt>> parse(const std::vector<Token>& tokens);

	private:
		std::unique_ptr<Stmt> stmt();
		std::unique_ptr<Stmt> exprStmt();

		std::unique_ptr<Expr> expr();
		
		std::unique_ptr<Expr> factor();
		std::unique_ptr<Expr> literal();

		void error(std::string_view message);
		const Token& peek() const;
		const Token& peekPrevious() const;
		void advance();
		bool isAtEnd();
		bool match(TokenType type);
		void expect(TokenType type, std::string_view errorMessage);

	private:
		const std::vector<Token>* m_tokens;
		size_t m_currentTokenIndex;
	};
}