#ifndef SELECTOR_COMMON_FACTORY_H_
#define SELECTOR_COMMON_FACTORY_H_

#include "logger.h"
#include "utils.h"

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

namespace selector
{
template<class ObjectType>
class Factory
{
public:
	using PointerType_t = std::shared_ptr<ObjectType>;
	using FactoryMethod_t = std::function<PointerType_t(void)>;

	Factory(const std::string& name) : m_name(name) { LOG_TRACE(Common, "Factory %s created", name.c_str()); }

	virtual ~Factory() {}

	PointerType_t Create(const std::string& name)
	{
		LOG_TRACE(Common, "Factory %s creating object of type %s", m_name.c_str(), name.c_str());

		auto methods = factoryMethods();
		auto iter = methods.find(name);
		if (iter == std::end(methods))
		{
			LOG_TRACE(Common, " Element of type %s not found. Returning nullptr.", name.c_str());
			return nullptr;
		}
		return iter->second();
	}

	std::vector<std::string> RegisteredTypes()
	{
		std::vector<std::string> typeNames;
		auto methods = factoryMethods();
		typeNames.reserve(methods.size());

		for (auto k : methods)
		{
			typeNames.push_back(k.first);
		}

		return typeNames;
	}

	void Register(const std::string& name, const FactoryMethod_t& method)
	{
		LOG_TRACE(Common, "Registering type %s with %s factory", name.c_str(), m_name.c_str());

		factoryMethods()[name] = method;
	}

private:
	auto& factoryMethods()
	{
		static std::unordered_map<std::string, FactoryMethod_t> s_factoryMethods;
		return s_factoryMethods;
	}

	std::string m_name;
};

}  // namespace selector

#endif
