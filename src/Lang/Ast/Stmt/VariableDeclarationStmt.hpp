#pragma once

#include <Lang/Ast/Stmt.hpp>
#include <Lang/Parsing/Token.hpp>

namespace Lang
{
	// Parsing complex data type names
	// How do compilers parse complex data types names like function pointers. The type has to be somehow put into the AST or it has
	// to be processed during parsing.
	// What are the pros and cons of different methods of parsing complex data types? What methods are most commonly used?
	// These are the ways I could think of
	// Do all the type checking in the parser (can't just parse only type names because of things like typedefs).
	// Use of intermediate structure to represent types in parser.
	// Get every token that is part of the type name and parse it during type checking.
	class VariableDeclarationStmt : public Stmt
	{
	public:
		// What to do with multi token types like function pointers? Type checking in the parser? :(
		// Should i just capture everything between ':' and (';' or '='). Create intermediate structure
		VariableDeclarationStmt(size_t start, size_t end);

		void accept(StmtVisitor& visitor) const override;
		StmtType type() const override;

	public:
		// Storing token so you can give errors about the type not being correct
		//const Token& name;
	};
}
