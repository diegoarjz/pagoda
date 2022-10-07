#pragma once

#include <pagoda/objects/procedural_operation.h>

#include <pagoda/math/plane.h>

namespace pagoda::geometry::operations
{
class ClipGeometry : public objects::ProceduralOperation
{
	public:
	static const std::string inputGeometry;
	static const std::string frontGeometry;
	static const std::string backGeometry;
	static const char* name;

	ClipGeometry(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~ClipGeometry();

	void Parameters(objects::NewParameterCallback* cb) override;

	const std::string& GetOperationName() const override;
	void Interfaces(objects::InterfaceCallback* cb) override;

	protected:
	void DoWork() override;

	private:
	objects::InterfacePtr m_inputGeometry;
	objects::InterfacePtr m_frontGeometry;
	objects::InterfacePtr m_backGeometry;

	math::Plane<float> m_plane;
};
}  // namespace pagoda::geometry::operations

