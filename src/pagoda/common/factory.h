#ifndef PAGODA_COMMON_FACTORY_H_
#define PAGODA_COMMON_FACTORY_H_

#include "pagoda/common/debug/logger.h"
#include "pagoda/common/utils.h"

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

namespace pagoda::common
{
template<class ObjectType>
class Factory
{
public:
	using PointerType_t = std::shared_ptr<ObjectType>;
	using FactoryMethod_t = std::function<PointerType_t(void)>;

	Factory(const std::string& name) : m_name(name) { LOG_TRACE(Common, "Factory " << name << " created"); }

	virtual ~Factory() {}

	PointerType_t Create(const std::string& name)
	{
		LOG_TRACE(Common, "Factory " << m_name << " creating object of type " << name)

		auto methods = factoryMethods();
		auto iter = methods.find(name);
		if (iter == std::end(methods))
		{
			LOG_TRACE(Common, " Element of type " << name << " not found. Returning nullptr.");
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
		LOG_TRACE(Common, "Registering type " << name << " with " << m_name << " factory");

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

}  // namespace pagoda::common

#endif
