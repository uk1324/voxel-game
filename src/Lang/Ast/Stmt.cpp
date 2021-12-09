#include <Lang/Ast/Stmt.hpp>

using namespace Lang;

Stmt::Stmt(size_t start, size_t end)
	: start(start)
	, end(end)
{}