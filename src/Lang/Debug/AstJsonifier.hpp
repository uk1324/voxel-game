#pragma once

#include <Lang/Ast/ExprVisitor.hpp>
#include <Lang/Ast/StmtVisitor.hpp>
#include <Json/Json.hpp>

namespace Lang
{
	class AstJsonifier : public ExprVisitor, public StmtVisitor
	{
	public:
		Json::Value jsonify(const std::vector<std::unique_ptr<Stmt>>& ast);

		void visitBinaryExpr(const BinaryExpr& expr) override;
		void visitNumberConstantExpr(const NumberConstantExpr& expr) override;

		void visitExprStmt(const ExprStmt& stmt) override;

	private:
		// Templated functions can't be virtual so I have to use this for return values.
		// I could use std::any.
		Json::Value m_returnValue;
	};
}