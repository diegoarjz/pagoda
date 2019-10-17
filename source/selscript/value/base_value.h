#pragma once

#include <memory>
#include <string>

namespace sscript
{
struct ValueVisitorBase;
class TypeInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;

struct BaseValue : public std::enable_shared_from_this<BaseValue>
{
	BaseValue(TypeInfoPtr type);
	virtual ~BaseValue() {}

	virtual std::string ToString() const = 0;
	virtual void AcceptVisitor(ValueVisitorBase *) = 0;

	TypeInfoPtr m_type;
};
using BaseValuePtr = std::shared_ptr<BaseValue>;
}  // namespace sscript
