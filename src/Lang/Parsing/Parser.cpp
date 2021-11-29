#include <Lang/Parsing/Parser.hpp>
#include <Utils/Assertions.hpp>
#include <Lang/Ast/Expr/BinaryExpr.hpp>
#include <Lang/Ast/Expr/NumberConstantExpr.hpp>
#include <Lang/Ast/Stmt/ExprStmt.hpp>

#include <iostream>

using namespace Lang;

Parser::Parser()
	: m_tokens(nullptr)
	, m_currentTokenIndex(0)
{}

std::vector<std::unique_ptr<Stmt>> Parser::parse(const std::vector<Token>& tokens)
{
	m_tokens = &tokens;
	m_currentTokenIndex = 0;

	std::vector<std::unique_ptr<Stmt>> ast;

	while (isAtEnd() == false)
	{
		ast.push_back(stmt());
	}

	return ast;
}

std::unique_ptr<Stmt> Parser::stmt()
{
	if (false)
	{

	}
	else
		return exprStmt();

}

std::unique_ptr<Stmt> Parser::exprStmt()
{
	auto expression = expr();
	auto stmt = std::make_unique<ExprStmt>(std::move(expression));
	expect(TokenType::Semicolon, "expected ';'");
	return stmt;
}

std::unique_ptr<Expr> Parser::expr()
{
	return factor();
}

std::unique_ptr<Expr> Parser::factor()
{
	auto expr = literal();

	while (match(TokenType::Plus) && (isAtEnd() == false))
	{
		Token op = peekPrevious();
		auto rhs = literal();
		expr = std::make_unique<BinaryExpr>(std::move(expr), std::move(rhs), op);
	}

	return expr;
}

std::unique_ptr<Expr> Parser::literal()
{
	if (match(TokenType::IntNumber))
	{
		return std::make_unique<NumberConstantExpr>(peekPrevious());
	}

	error("expected literal");
}

void Parser::error(std::string_view message)
{
	std::cout << "Syntax Error: " << message << '\n';
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

void Parser::expect(TokenType type, std::string_view errorMessage)
{
	if (match(type) == false)
	{
		error(errorMessage);
	}
}

void Parser::advance()
{
	if (isAtEnd() == false)
		m_currentTokenIndex++;
}

bool Parser::isAtEnd()
{
	// Shouldn't advance past equality. Probably could remove this.
	return (peek().type == TokenType::Eof);
}