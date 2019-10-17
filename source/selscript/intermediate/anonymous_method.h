#pragma once

#include "ast.h"

#include <memory>

namespace sscript
{
namespace ast
{
class AnonymousMethod : public Expression
{
public:
	AnonymousMethod() = default;
	AnonymousMethod(const ExpressionPtr &instance, const StatementBlockPtr body);
	virtual ~AnonymousMethod();

	ExpressionPtr GetInstance() const { return m_instance; }
	StatementBlockPtr GetBody() const { return m_body; }

	void AcceptVisitor(AstVisitor *v) override;

private:
	ExpressionPtr m_instance;
	StatementBlockPtr m_body;
};
using AnonymousMethodPtr = std::shared_ptr<AnonymousMethod>;
}  // namespace ast
}  // namespace sscript
