#pragma once

#include <Lang/Ast/Expr.hpp>
#include <Lang/Ast/Stmt.hpp>
#include <Lang/Parsing/Token.hpp>
#include <Lang/Parsing/SourceInfo.hpp>
#include <Utils/SmartPointers.hpp>

#include <vector>

namespace Lang
{
	class Parser
	{
	private:
		// Maybe later add an optional help parameter like rust
		class ParsingError
		{
		public:
			// Using const char* for error messages so I don't have to worry about string overship
			ParsingError(const char* message, size_t start, size_t end);
			ParsingError(const char* message, const Token& token);
			ParsingError(const char* message, const Expr& expr);
			ParsingError(const char* message, const Stmt& stmt);

		public:
			const char* const message;
			size_t start;
			size_t length;
		};

	public:
		Parser();

		// Could use move here on return.
		std::vector<OwnPtr<Stmt>> parse(const std::vector<Token>& tokens, const SourceInfo& sourceInfo);

	private:
		OwnPtr<Stmt> stmt();
		OwnPtr<Stmt> exprStmt();
		OwnPtr<Stmt> printStmt();

		OwnPtr<Expr> expr();
		
		OwnPtr<Expr> factor();
		

		OwnPtr<Expr> primary();

		const Token& peek() const;
		const Token& peekPrevious() const;
		void advance();
		bool isAtEnd();
		bool match(TokenType type);
		// Don't know if I should automate expect expressions sometimes you can provide additional information why or where the error occured
		// For example in c++ "expected ';' after struct declaration". Errors like this could also be automated or this could be 
		// made an exception. To automate this you would need to change ParsingError constructor to use std::string instead of const char* 
		// or do it at compile time maybe with macros idk.
		void expect(TokenType type, const char* errorMessage);
		void synchronize();

	private:
		const std::vector<Token>* m_tokens;
		size_t m_currentTokenIndex;

		const SourceInfo* m_sourceInfo;
	};
}