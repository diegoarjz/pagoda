#ifndef SELECTOR_PROCEDURAL_OBJECTS_PROCEDURAL_OPERATION_FACTORY_H_
#define SELECTOR_PROCEDURAL_OBJECTS_PROCEDURAL_OPERATION_FACTORY_H_

#include <memory>
#include <vector>
#include <unordered_map>

namespace selector
{
class ProceduralOperation;
using ProceduralOperationPtr = std::shared_ptr<ProceduralOperation>;

enum class ProceduralOperationType
{
	Extrude,
	CreateRect,
	Triangulate,
	Max
};

class ProceduralOperationFactoryMethod
{
public:
    virtual ~ProceduralOperationFactoryMethod() {}
    virtual ProceduralOperationPtr Create() = 0;
};

class ProceduralOperationFactory
{
public:
	static ProceduralOperationPtr Create(ProceduralOperationType type);
    static ProceduralOperationFactory* Instance();

    ProceduralOperationPtr Create(const char *name);
    void Register(const char *name, ProceduralOperationFactoryMethod *factoryMethod);
    std::vector<const char *> RegisteredProceduralOperations() const;

private:
    ProceduralOperationFactory();
    ProceduralOperationFactory(const ProceduralOperationFactory&) = delete;
    ProceduralOperationFactory& operator=(const ProceduralOperationFactory&) = delete;

    std::unordered_map<const char *, ProceduralOperationFactoryMethod*> m_factoryMethods;
};  // class ProceduralOperationFactory

#define REGISTER_PROCEDURAL_OPERATION(klass) \
    class klass##Factory : public selector::ProceduralOperationFactoryMethod \
    { \
    public: \
        klass##Factory() \
        { \
            selector::ProceduralOperationFactory::Instance()->Register(#klass, this); \
        } \
        selector::ProceduralOperationPtr Create() override { return std::make_shared<klass>(); } \
    }; \
    static klass##Factory global_##klass##Factory;

}  // namespace selector

#endif
