#ifndef PAGODA_COMMON_FACTORY_H_
#define PAGODA_COMMON_FACTORY_H_

#include "pagoda/common/debug/logger.h"
#include "pagoda/common/utils.h"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace pagoda::common {
template <class ObjectType, class KeyType = std::string> class Factory {
public:
  using PointerType_t = std::shared_ptr<ObjectType>;
  using FactoryMethod_t = std::function<PointerType_t(void)>;

  Factory(const std::string &name) : m_name(name) {}

  virtual ~Factory() {}

  virtual PointerType_t Create(const KeyType &name) {
    auto methods = factoryMethods();
    auto iter = methods.find(name);
    if (iter == std::end(methods)) {
      return nullptr;
    }
    return iter->second();
  }

  std::vector<KeyType> RegisteredTypes() {
    std::vector<KeyType> typeNames;
    auto methods = factoryMethods();
    typeNames.reserve(methods.size());

    for (auto k : methods) {
      typeNames.push_back(k.first);
    }

    return typeNames;
  }

  void Register(const KeyType &name, const FactoryMethod_t &method) {
    factoryMethods()[name] = method;
  }

  const std::string& GetName() { return m_name; }

protected:
  auto &factoryMethods() {
    static std::unordered_map<KeyType, FactoryMethod_t> s_factoryMethods;
    return s_factoryMethods;
  }

  std::string m_name;
};

} // namespace pagoda::common

#endif
