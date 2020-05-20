#pragma once

#include "class_base.h"
#include "dynamic_value_base.h"

namespace pagoda::dynamic
{
class TypeInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;

class DynamicClass;
using DynamicClassPtr = std::shared_ptr<DynamicClass>;

class Function;
using FunctionPtr = std::shared_ptr<Function>;

class ICallableBody;

class DynamicValueTable;

/**
 * Represents a dynamic instance that can be created in pgscript.
 */
class DynamicInstance : public DynamicValueBase, public ClassBase
{
public:
	static const TypeInfoPtr s_typeInfo;

	explicit DynamicInstance(DynamicClassPtr klass);
	virtual ~DynamicInstance();

	std::string ToString() const override;

	void AcceptVisitor(ValueVisitorBase& visitor) override;
	FunctionPtr Bind(std::shared_ptr<ICallableBody> callable,
	                 std::shared_ptr<DynamicValueTable> globals = nullptr) override;

private:
	DynamicClassPtr m_class;
};

using DynamicInstancePtr = std::shared_ptr<DynamicInstance>;
}  // namespace pagoda::dynamic
