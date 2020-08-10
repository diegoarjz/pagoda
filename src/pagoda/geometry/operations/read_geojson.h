#pragma once

#include <pagoda/objects/procedural_operation.h>

#include "pagoda/objects/procedural_object_system.h"
namespace pagoda::geometry::operations
{
class ReadGeoJson : public objects::ProceduralOperation
{
public:
	static const std::string outputGeometry;

	ReadGeoJson(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~ReadGeoJson();

	void DoWork() override;
};
}  // namespace pagoda::geometry::operations
