#define SELECTOR_STATISTICS_ENABLED

#include "../performance_test.h"

#include <common/profiler.h>
#include <common/statistics.h>
#include <geometry_core/geometry_exporter.h>
#include <geometry_core/geometry_topology.h>
#include <geometry_operations/create_rect.h>
#include <geometry_operations/extrusion.h>
#include <math_lib/vec_base.h>

#include <cstring>
#include <fstream>

using namespace selector;

using GeometryType = GeometryT<SplitPointTopology<uint32_t>>;
using GeometryPtr = std::shared_ptr<GeometryType>;

std::string PerformanceTest::TestName() { return __FILE__; }

const char* extrusion_quad = "Extrusion Quad";
const char* compound_quad_extrusion = "Compounded Quad Extrusion";
const char* create_rect = "Create Rect";

void ExportGeometry(GeometryPtr geom, const std::string& file)
{
	if (!geom)
	{
		return;
	}

	ObjExporter<GeometryType> exporter(geom);
	std::ofstream out_file(file);
	exporter.Export(out_file);
}

void RunQuadExtrusion()
{
	// Quad
	GeometryPtr in = std::make_shared<GeometryType>(4, 1, 5);
	Vec3F points[] = {Vec3F(0, 0, 0), Vec3F(1, 0, 0), Vec3F(1, 1, 0), Vec3F(0, 1, 0)};
	for (auto& v : points)
	{
		in->CreateVertex(v);
	}
	in->CreateFace(std::vector<uint32_t>{
	    0,
	    1,
	    2,
	    3,
	});

	Extrusion<GeometryType> extrusion(1.0f);
	GeometrySizes sizes = extrusion.ResultSize(in);

	GeometryPtr out = std::make_shared<GeometryType>(sizes.m_numVertices, sizes.m_numEdges, sizes.m_numFaces);
	{
		OneShotProfiler p(extrusion_quad);
		extrusion.Execute(in, out);
	}

	ExportGeometry(out, "geom.obj");
}

void RunCompoundQuadExtrusion()
{
	// Quad
	GeometryPtr in = std::make_shared<GeometryType>(4, 1, 5);
	Vec3F points[] = {Vec3F(0, 0, 0), Vec3F(1, 0, 0), Vec3F(1, 1, 0), Vec3F(0, 1, 0)};
	for (auto& v : points)
	{
		in->CreateVertex(v);
	}
	in->CreateFace(std::vector<uint32_t>{
	    0,
	    1,
	    2,
	    3,
	});

	GeometryPtr out = nullptr;
	{
		OneShotProfiler p(compound_quad_extrusion);

		// TODO: it could be interesting to make benchmarks out of this sort of things.
		//       See how many compound extrusions the system can do before timing out
		Extrusion<GeometryType> extrusion(1.0f);
		out = in;
		for (auto i = 0u; i < 5; ++i)
		{
			GeometrySizes sizes = extrusion.ResultSize(in);
			out = std::make_shared<GeometryType>(sizes.m_numVertices, sizes.m_numEdges, sizes.m_numFaces);
			extrusion.Execute(in, out);
			in = out;
		}
	}

	ExportGeometry(out, "geom.obj");
}

void RunCreateRect()
{
	CreateRect<GeometryType> create(10, 10);

	GeometrySizes sizes = create.ResultSize();
	GeometryPtr out = std::make_shared<GeometryType>(sizes.m_numVertices, sizes.m_numEdges, sizes.m_numFaces);
	{
		OneShotProfiler p(create_rect);
		create.Execute(out);
	}

	ExportGeometry(out, "geom.obj");
}

void PerformanceTest::Run(int argc, char* argv[])
{
	if (argc < 2)
	{
		return;
	}

	if (strcmp(argv[1], "quad_extrusion") == 0)
	{
		RunQuadExtrusion();
	}
	else if (strcmp(argv[1], "compound_quad_extrusion") == 0)
	{
		RunCompoundQuadExtrusion();
	}
	else if (strcmp(argv[1], "create_rect") == 0)
	{
		RunCreateRect();
	}
}
