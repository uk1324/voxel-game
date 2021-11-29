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
	node["type"] = expr.name();
	node["value"] = std::string(expr.number.text);
	m_returnValue = std::move(node);
}

void AstJsonifier::visitExprStmt(const ExprStmt& stmt)
{
	Json::Value node = Json::Value::emptyObject();
	node["type"] = stmt.name();
	stmt.expr->accept(*this);
	node["expression"] = m_returnValue;
	m_returnValue = std::move(node);
}
