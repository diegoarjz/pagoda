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

	void SetParameters(graph::ExecutionArgumentCallback* cb) override;

	const std::string& GetOperationName() const override;

	void DoWork() override;
};
}  // namespace pagoda::geometry::operations

#endif
