#pragma once

#include "../binding/make_builtin_function.h"
#include "../value/builtin_class.h"
#include "../value/builtin_member_function.h"

namespace sscript
{
template<typename T>
class make_builtin_class
{
public:
	make_builtin_class(const std::string& className) : m_class(std::make_shared<BuiltinClass<T>>(className)) {}

	template<typename... Args>
	make_builtin_class& set_constructor(std::function<std::shared_ptr<BuiltinInstance<T>>(Args...)>&& ctor)
	{
		auto function = make_builtin_function(m_class->GetCallableName(), ctor);
		m_class->SetConstructor(function);
		return *this;
	}

	template<typename R, typename... Args>
	make_builtin_class& add_method(const std::string& name, std::function<R(Args...)>&& member)
	{
		auto builtinMember = std::make_shared<BuiltinMemberFunction<T, std::function<R(Args...)>>>(member);
		m_class->GetSymbolTable()->Declare({name, builtinMember});
		return *this;
	}

	std::shared_ptr<BuiltinClass<T>> make() { return m_class; }

private:
	std::shared_ptr<BuiltinClass<T>> m_class;
};
}  // namespace sscript
