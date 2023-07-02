#pragma once

#include "factory.h"
#include "plugin.h"

namespace pagoda::common {
template <class ObjectType, class KeyType = std::string>
class PluggableFactory : public Factory<ObjectType, KeyType> {
public:
  using BaseClass = Factory<ObjectType, KeyType>;
  using PointerType_t = typename BaseClass::PointerType_t;

  class Registration {
  public:
    virtual void Register(PluggableFactory<ObjectType, KeyType> *) = 0;
  };

  PluggableFactory(const std::string &name) : BaseClass(name) {}
  ~PluggableFactory() override = default;

  PointerType_t Create(const KeyType &name) override {
    LoadPlugins();
    return BaseClass::Create(name);
  }

private:
  void LoadPlugins() {
    if (!m_pluginsLoaded) {
      auto &registry = PluginRegistry::Instance();
      const auto &plugins =
          registry.GetPluginsForRegistry(BaseClass::GetName());
      for (const auto &p : plugins) {
        if (auto regClass = p->template GetRegistrationClass<Registration>()) {
          regClass->Register(this);
        }
      }
    }
    m_pluginsLoaded = true;
  }

  bool m_pluginsLoaded{false};
};
} // namespace pagoda::common
