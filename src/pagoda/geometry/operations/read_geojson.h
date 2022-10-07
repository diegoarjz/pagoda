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

	void Parameters(objects::NewParameterCallback* cb) override;

	const std::string& GetOperationName() const override;
	void Interfaces(objects::InterfaceCallback* cb) override;

	void DoWork() override;

	private:
	objects::InterfacePtr m_output;

	std::string m_file;
	float m_refLatitude;
	float m_refLongitude;
};
}  // namespace pagoda::geometry::operations
