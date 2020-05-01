#pragma once

#include "class_base.h"
#include "dynamic_value_base.h"
#include "icallable.h"

namespace pagoda
{
class TypeInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;

class ICallableBody;

/**
 * Represents a dynamic class that can be defined in pgscript.
 */
class DynamicClass : public DynamicValueBase, public ClassBase, public ICallable
{
public:
	static const TypeInfoPtr typeInfo;

	explicit DynamicClass(const std::string& className);
	virtual ~DynamicClass();

	const std::string& GetClassName() const;

	void SetConstructorBody(std::shared_ptr<ICallableBody> callableBody);

	/*
	 * DynamicValueBase overrides
	 */
	std::string ToString() const override;
	void AcceptVisitor(ValueVisitorBase& visitor) override;

	/*
	 * ICallable overrides
	 */
	void SetCallableName(const std::string&) override;
	const std::string& GetCallableName() const override;

	void SetArity(const std::size_t&) override;
	const std::size_t& GetArity() const override;
	bool IsVariadic() const override;
	void SetVariadic(bool) override;
	void SetClosure(const std::shared_ptr<DynamicValueTable>&) override;
	const std::shared_ptr<DynamicValueTable>& GetClosure() const override;

	DynamicValueBasePtr Call(const std::vector<DynamicValueBasePtr>& args) override;

	FunctionPtr Bind(std::shared_ptr<ICallableBody> callable, std::shared_ptr<DynamicValueTable> globals = nullptr) override;

private:
	std::string m_className;
	std::size_t m_arity;
	bool m_isVariadic;
	DynamicValueTablePtr m_closure;
	std::shared_ptr<ICallableBody> m_callableBody;
};

using DynamicClassPtr = std::shared_ptr<DynamicClass>;
}  // namespace pagoda
