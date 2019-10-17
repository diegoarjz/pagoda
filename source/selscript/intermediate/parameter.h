#pragma once

#include "ast.h"

namespace sscript
{
namespace ast
{
class Parameter : public AstNode
{
public:
	explicit Parameter(const std::string& identifier);
	virtual ~Parameter() {}

	const std::string& GetIdentifier() const { return m_identifierName; }
	void SetParameterType(const std::string& type) { m_typeName = type; }
	const boost::optional<std::string>& GetParameterType() const { return m_typeName; }

	void AcceptVisitor(AstVisitor*) override;

private:
	std::string m_identifierName;
    boost::optional<std::string> m_typeName;
};
using ParameterPtr = std::shared_ptr<Parameter>;
}  // namespace ast
}  // namespace sscript
