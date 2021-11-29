#pragma once

namespace Lang
{
	class StmtVisitor;

	class Stmt
	{
	public:
		virtual ~Stmt() = default;

		virtual void accept(StmtVisitor& visitor) const = 0;
		virtual const char* name() const = 0;
	};
}