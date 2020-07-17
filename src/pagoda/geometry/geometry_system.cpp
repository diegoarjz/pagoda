#include "geometry_system.h"

#include "operations/clip_geometry.h"
#include "operations/create_box.h"
#include "operations/create_rect.h"
#include "operations/create_sphere.h"
#include "operations/export_geometry.h"
#include "operations/extract_faces.h"
#include "operations/extract_scope.h"
#include "operations/extrude_geometry.h"
#include "operations/face_offset.h"
#include "operations/repeat_split.h"
#include "operations/rotate.h"
#include "operations/scale.h"
#include "operations/scope_texture_projection.h"
#include "operations/split.h"
#include "operations/translate.h"
#include "operations/triangulate_geometry.h"

#include <pagoda/geometry/geometry_component.h>
#include <memory>

#include <pagoda/objects/procedural_object.h>

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/instrument/profiler.h>

#include <pagoda/pagoda.h>

namespace pagoda::geometry
{
using namespace operations;

const std::string GeometrySystem::GetComponentSystemName() { return "GeometrySystem"; }

GeometrySystem::GeometrySystem() : ProceduralComponentSystem(GetComponentSystemName()) {}
GeometrySystem::~GeometrySystem() {}

void GeometrySystem::DoClone(std::shared_ptr<GeometryComponent> from, std::shared_ptr<GeometryComponent> to)
{
	auto newGeom = std::make_shared<Geometry>();
	to->SetGeometry(newGeom);
	*newGeom = *(from->GetGeometry());
	to->SetScope(from->GetScope());
}

void GeometrySystem::Registration(Pagoda* pagoda)
{
	LOG_TRACE(Core, "Registering " << GetComponentSystemName());

	auto operationFactory = pagoda->GetOperationFactory();
	auto objectSystem = pagoda->GetProceduralObjectSystem();

	objectSystem->RegisterProceduralComponentSystem(std::make_shared<GeometrySystem>());

	// Register Operations
	{
		operationFactory->Register("CreateRectGeometry",
		                           [objectSystem]() { return std::make_shared<CreateRectGeometry>(objectSystem); });
		operationFactory->Register("CreateSphereGeometry",
		                           [objectSystem]() { return std::make_shared<CreateSphereGeometry>(objectSystem); });
		operationFactory->Register("CreateBoxGeometry",
		                           [objectSystem]() { return std::make_shared<CreateBoxGeometry>(objectSystem); });
		operationFactory->Register("ExportGeometry",
		                           [objectSystem]() { return std::make_shared<ExportGeometry>(objectSystem); });
		operationFactory->Register("ExtractScope",
		                           [objectSystem]() { return std::make_shared<ExtractScope>(objectSystem); });
		operationFactory->Register("ExtrudeGeometry",
		                           [objectSystem]() { return std::make_shared<ExtrudeGeometry>(objectSystem); });
		operationFactory->Register("FaceOffset",
		                           [objectSystem]() { return std::make_shared<FaceOffsetOperation>(objectSystem); });
		operationFactory->Register("TriangulateGeometry",
		                           [objectSystem]() { return std::make_shared<TriangulateGeometry>(objectSystem); });
		operationFactory->Register("ClipGeometry",
		                           [objectSystem]() { return std::make_shared<ClipGeometry>(objectSystem); });
		operationFactory->Register("RepeatSplit",
		                           [objectSystem]() { return std::make_shared<RepeatSplit>(objectSystem); });
		operationFactory->Register("ExtractFaces",
		                           [objectSystem]() { return std::make_shared<ExtractFaces>(objectSystem); });
		operationFactory->Register("Translate", [objectSystem]() { return std::make_shared<Translate>(objectSystem); });
		operationFactory->Register("Split", [objectSystem]() { return std::make_shared<Split>(objectSystem); });
		operationFactory->Register("Scale", [objectSystem]() { return std::make_shared<Scale>(objectSystem); });
		operationFactory->Register("Rotate", [objectSystem]() { return std::make_shared<Rotate>(objectSystem); });
		operationFactory->Register("ScopeTextureProjection",
		                           [objectSystem]() { return std::make_shared<ScopeTextureProjection>(objectSystem); });
	}
}
}  // namespace pagoda::geometry
