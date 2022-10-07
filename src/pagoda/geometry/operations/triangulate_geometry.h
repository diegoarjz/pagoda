#pragma once
#ifndef PAGODA_PROCEDURAL_OBJECT_TRIANGULATE_GEOMETRY_H_
#define PAGODA_PROCEDURAL_OBJECT_TRIANGULATE_GEOMETRY_H_

#include <pagoda/objects/procedural_operation.h>

namespace pagoda::geometry::operations
{
class TriangulateGeometry : public objects::ProceduralOperation
{
	public:
	static const std::string sInputGeometry;
	static const std::string sOutputGeometry;
	static const char* name;

	TriangulateGeometry(objects::ProceduralObjectSystemPtr objectSystem);
	virtual ~TriangulateGeometry();


	const std::string& GetOperationName() const override;
	void Interfaces(objects::InterfaceCallback* cb) override;

	void DoWork() override;

	private:
	objects::InterfacePtr m_input;
	objects::InterfacePtr m_output;
};
}  // namespace pagoda::geometry::operations

#endif
