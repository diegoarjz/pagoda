#ifndef SELECTOR_COMMON_FACTORY_H_
#define SELECTOR_COMMON_FACTORY_H_

#include "logger.h"
#include "utils.h"

#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>

namespace selector
{
template<class Base>
class Factory
{
public:
	using PointerType = std::shared_ptr<Base>;

	virtual ~Factory() {}

	static PointerType Create(const char* name)
	{
		// TODO: Better error checking
		return factoryMethods().at(name)();
	}

	static std::vector<const char*> RegisteredTypes()
	{
		std::vector<const char*> typeNames;
		auto methods = factoryMethods();
		typeNames.reserve(methods.size());

		for (auto k : methods)
		{
			typeNames.push_back(k.first);
		}

		return typeNames;
	}

	template<class T>
	struct Registrar : public Base
	{
		friend T;

		static bool Register()
		{
			const auto name = T::name;
			Factory::factoryMethods()[name] = []() -> PointerType { return std::make_shared<T>(); };
			return true;
		}

		static const bool registered;

	private:
		Registrar() { (void)(registered); }
	};

	friend Base;

private:
	using FactoryMethod = std::function<PointerType(void)>;

	Factory() = default;

	static auto& factoryMethods()
	{
		static std::unordered_map<const char*, FactoryMethod> s_factoryMethods;
		return s_factoryMethods;
	}
};

template<class Base>
template<class T>
const bool Factory<Base>::Registrar<T>::registered = Factory<Base>::Registrar<T>::Register();

template<class T>
bool IsRegistered()
{
	return T::registered;
}
}  // namespace selector

#endif
