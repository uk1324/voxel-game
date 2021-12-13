#include <Lang/Parsing/Parser.hpp>
#include <Lang/Ast/Expr/BinaryExpr.hpp>
#include <Lang/Ast/Expr/IntConstantExpr.hpp>
#include <Lang/Ast/Stmt/ExprStmt.hpp>
#include <Lang/Ast/Stmt/PrintStmt.hpp>
#include <Utils/Assertions.hpp>
#include <Utils/TerminalColors.hpp>

#include <iostream>

using namespace Lang;

Parser::ParsingError::ParsingError(const char* message, size_t start, size_t length)
	: message(message)
	, start(start)
	, length(length)
{}

Parser::ParsingError::ParsingError(const char* message, const Token& token)
	: ParsingError(message, token.start, token.end - token.start)
{}

Parser::ParsingError::ParsingError(const char* message, const Expr& expr)
	: ParsingError(message, expr.start, expr.length)
{}

Parser::ParsingError::ParsingError(const char* message, const Stmt& stmt)
	: ParsingError(message, stmt.start, stmt.length)
{}

Parser::Parser()
	: m_tokens(nullptr)
	, m_currentTokenIndex(0)
	, m_sourceInfo(nullptr)
{}

std::vector<OwnPtr<Stmt>> Parser::parse(const std::vector<Token>& tokens, const SourceInfo& sourceInfo)
{
	m_tokens = &tokens;
	m_sourceInfo = &sourceInfo;
	m_currentTokenIndex = 0;

	std::vector<OwnPtr<Stmt>> ast;

	while (isAtEnd() == false)
	{
		try
		{
			if (match(TokenType::Semicolon))
				; // Null statement like this line
			else
				ast.push_back(stmt());
		}
		catch (const ParsingError& error)
		{
			// if (isAtEnd()) // EOF error // Should there be eof errors

			// Skip errors from scanner.
			if (peek().type == TokenType::Error)
			{
				;
			}
			else
			{
				// GCC multi line error example
				/*
				source>:19:9: error: cannot convert 'X' to 'int'
			   19 |         X(
				  |         ^~
				  |         |
				  |         X
			   20 | 
				  |          
			   21 |         )
				  |         ~
				*/
				std::cout << TERM_COL_RED << "error: " << TERM_COL_RESET << error.message << '\n'
					<< m_sourceInfo->getLineText(m_sourceInfo->getLine(error.start));
			}

			synchronize();
		}
	}

	return ast;
}

OwnPtr<Stmt> Parser::stmt()
{
	if (match(TokenType::Print))
		return printStmt();
	else
		return exprStmt();

}

OwnPtr<Stmt> Parser::exprStmt()
{
	size_t start = peek().start;
	auto expression = expr();
	expect(TokenType::Semicolon, "expected ';'");
	size_t end = peekPrevious().end;

	auto stmt = std::make_unique<ExprStmt>(std::move(expression), start, end);
	return stmt;
}

OwnPtr<Stmt> Parser::printStmt()
{
	size_t start = peekPrevious().start;
	expect(TokenType::LeftParen, "expected '('");
	auto expression = expr();
	expect(TokenType::RightParen, "expected ')'");
	expect(TokenType::Semicolon, "expected ';'");
	size_t end = peek().end;
	auto stmt = std::make_unique<PrintStmt>(std::move(expression), start, end);
	return stmt;
}

OwnPtr<Expr> Parser::expr()
{
	return factor();
}

OwnPtr<Expr> Parser::factor()
{
	size_t start = peek().start;
	auto expr = primary();

	while (match(TokenType::Plus) && (isAtEnd() == false))
	{
		Token op = peekPrevious();
		auto rhs = primary();
		size_t end = peekPrevious().end;
		expr = std::make_unique<BinaryExpr>(std::move(expr), std::move(rhs), op, start, end);
	}

	return expr;
}

OwnPtr<Expr> Parser::primary()
{
	if (match(TokenType::IntNumber))
	{
		return std::make_unique<IntConstantExpr>(peekPrevious(), peekPrevious().start, peekPrevious().end);
	}

	throw ParsingError("expected primary expression", peek().start, peek().start);
}

const Token& Parser::peek() const
{
	return (*m_tokens)[m_currentTokenIndex];
}

const Token& Parser::peekPrevious() const
{
	return (*m_tokens)[m_currentTokenIndex - 1];
}

bool Parser::match(TokenType type)
{
	if (peek().type == type)
	{
		advance();
		return true;
	}
	return false;
}

void Parser::expect(TokenType type, const char* errorMessage)
{
	if (match(type) == false)
	{
		throw ParsingError(errorMessage, peek().start, peek().end);
	}
}

void Parser::synchronize()
{
	while (isAtEnd() == false)
	{
		switch (peek().type)
		{
			case TokenType::Semicolon:
				return;

			default:
				advance();
				break;
		}
	}
}

void Parser::advance()
{
	if (isAtEnd() == false)
		m_currentTokenIndex++;
}

bool Parser::isAtEnd()
{
	return (peek().type == TokenType::Eof);
}