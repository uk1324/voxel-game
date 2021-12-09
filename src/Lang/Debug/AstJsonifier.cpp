#include "AstJsonifier.hpp"
#include "AstJsonifier.hpp"
#include <Lang/Debug/AstJsonifier.hpp>

using namespace Lang;

// Maybe make function for accept and return.

Json::Value AstJsonifier::jsonify(const std::vector<std::unique_ptr<Stmt>>& ast)
{
	Json::Value json = Json::Value::emptyArray();

	for (const auto& stmt : ast)
	{
		stmt->accept(*this);
		json.array().push_back(std::move(m_returnValue));
	}
	
	return json;
}

void AstJsonifier::visitBinaryExpr(const BinaryExpr& expr)
{
	Json::Value node = Json::Value::emptyObject();
	addStartEnd(node, expr);
	node["type"] = expr.name();
	node["operator"] = std::string(expr.op.text);
	expr.lhs->accept(*this);
	node["left"] = m_returnValue;
	expr.rhs->accept(*this);
	node["right"] = m_returnValue;
	m_returnValue = std::move(node);
}

void AstJsonifier::visitNumberConstantExpr(const NumberConstantExpr& expr)
{
	Json::Value node = Json::Value::emptyObject();
	addStartEnd(node, expr);
	node["type"] = expr.name();
	node["value"] = std::string(expr.number.text);
	m_returnValue = std::move(node);
}

void AstJsonifier::visitExprStmt(const ExprStmt& stmt)
{
	Json::Value node = Json::Value::emptyObject();
	addStartEnd(node, stmt);
	node["type"] = stmt.name();
	stmt.expr->accept(*this);
	node["expression"] = m_returnValue;
	m_returnValue = std::move(node);
}

void AstJsonifier::addStartEnd(Json::Value& node, const Expr& expr)
{
	node["start"] = Json::Value::IntType(expr.start);
	node["end"] = Json::Value::IntType(expr.end);
}

void Lang::AstJsonifier::addStartEnd(Json::Value& node, const Stmt& stmt)
{
	node["start"] = Json::Value::IntType(stmt.start);
	node["end"] = Json::Value::IntType(stmt.end);
}
