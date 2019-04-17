#pragma once
#ifndef SELECTOR_PROCEDURAL_OBJECT_TRIANGULATE_GEOMETRY_H_
#define SELECTOR_PROCEDURAL_OBJECT_TRIANGULATE_GEOMETRY_H_

#include "procedural_operation.h"

namespace selector
{
class TriangulateGeometry : public ProceduralOperation::Registrar<TriangulateGeometry>
{
public:
	static const InterfaceName sInputGeometry;
	static const InterfaceName sOutputGeometry;
	static const char* name;

	TriangulateGeometry();
	virtual ~TriangulateGeometry();

	void Execute() override;
};
}  // namespace selector

#endif
