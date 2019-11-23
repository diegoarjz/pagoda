#pragma once

namespace selector
{
template<class C, class F>
void RegisterMemberFunction(C* instance, const std::string& name, const F& memberFunction)
{
	instance->RegisterMember(name, std::make_shared<Function>(std::make_shared<MemberFunctionCallableBody<C, F>>(
	                                   *static_cast<C*>(instance), memberFunction)));
}
}  // namespace selector
