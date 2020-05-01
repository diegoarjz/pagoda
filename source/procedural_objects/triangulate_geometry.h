#pragma once
#ifndef PAGODA_PROCEDURAL_OBJECT_TRIANGULATE_GEOMETRY_H_
#define PAGODA_PROCEDURAL_OBJECT_TRIANGULATE_GEOMETRY_H_

#include "procedural_operation.h"

namespace pagoda
{
class TriangulateGeometry : public ProceduralOperation
{
public:
	static const std::string sInputGeometry;
	static const std::string sOutputGeometry;
	static const char* name;

	TriangulateGeometry(ProceduralObjectSystemPtr objectSystem);
	virtual ~TriangulateGeometry();

	void DoWork() override;
};
}  // namespace pagoda

#endif
