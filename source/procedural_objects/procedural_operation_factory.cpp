#include "procedural_operation_factory.h"

#include "create_rect.h"
#include "extrude_geometry.h"
#include "triangulate_geometry.h"

namespace selector
{
ProceduralOperationPtr ProceduralOperationFactory::Create(ProceduralOperationType type)
{
	switch (type)
	{
		case ProceduralOperationType::Extrude:
			return std::make_shared<ExtrudeGeometry>();
		case ProceduralOperationType::CreateRect:
			return std::make_shared<CreateRectGeometry>();
		case ProceduralOperationType::Triangulate:
			return std::make_shared<TriangulateGeometry>();
		default:
			return nullptr;
	}
}

ProceduralOperationFactory::ProceduralOperationFactory() {}

ProceduralOperationFactory* ProceduralOperationFactory::Instance()
{
    static ProceduralOperationFactory factory;
    return &factory;
}

ProceduralOperationPtr ProceduralOperationFactory::Create(const char *name)
{
    if (m_factoryMethods.find(name) == std::end(m_factoryMethods))
    {
        return nullptr;
    }

    return m_factoryMethods[name]->Create();
}

void ProceduralOperationFactory::Register(const char *name, ProceduralOperationFactoryMethod *factoryMethod)
{
    m_factoryMethods[name] = factoryMethod;
}

std::vector<const char *> ProceduralOperationFactory::RegisteredProceduralOperations() const
{
    std::vector<const char*> operationNames;
    operationNames.reserve(m_factoryMethods.size());

    for (auto k : m_factoryMethods)
    {
        operationNames.push_back(k.first);
    }

    return operationNames;
}

}  // namespace selector
