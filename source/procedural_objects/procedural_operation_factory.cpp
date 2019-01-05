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

}  // namespace selector
