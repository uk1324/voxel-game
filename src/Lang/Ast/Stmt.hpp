#pragma once

namespace Lang
{
	class StmtVisitor;

	class Stmt
	{
	public:
		Stmt(size_t start, size_t end);
		virtual ~Stmt() = default;

		virtual void accept(StmtVisitor& visitor) const = 0;
		virtual const char* name() const = 0;

	public:
		size_t start;
		size_t end;
	};
}