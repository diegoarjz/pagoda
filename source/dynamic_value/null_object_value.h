#pragma once

#include "dynamic_value_base.h"

namespace selector
{
class TypeInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;

class NullObject : public DynamicValueBase
{
public:
	static const TypeInfoPtr s_typeInfo;

	NullObject();
	virtual ~NullObject();

	std::string ToString() const override;

	void AcceptVisitor(ValueVisitorBase& visitor) override;
};
using NullObjectPtr = std::shared_ptr<NullObject>;

}  // namespace selector
