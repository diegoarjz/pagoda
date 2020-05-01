#pragma once

#include "dynamic_value_base.h"
#include "icallable.h"
#include "icallable_body.h"

namespace pagoda
{
class Function : public DynamicValueBase, public ICallable
{
public:
	static const TypeInfoPtr s_typeInfo;

	/**
	 * Constructs with a \c ICallableBody.
	 */
	Function(std::shared_ptr<ICallableBody> callableBody);

	std::shared_ptr<ICallableBody> GetCallableBody() const;
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
	DynamicValueBasePtr Call(const std::vector<DynamicValueBasePtr>& args) override;
	void SetArity(const std::size_t&) override;
	const std::size_t& GetArity() const override;
	bool IsVariadic() const override;
	void SetVariadic(bool) override;
	void SetClosure(const std::shared_ptr<DynamicValueTable>&) override;
	const std::shared_ptr<DynamicValueTable>& GetClosure() const override;

private:
	std::shared_ptr<ICallableBody> m_callableBody;
	std::string m_callableName;
	std::size_t m_arity;
	bool m_isVariadic;
};
}  // namespace pagoda
