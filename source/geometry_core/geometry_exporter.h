#ifndef SELECTOR_GEOMETRY_CORE_GEOMETRY_EXPORTER_H_
#define SELECTOR_GEOMETRY_CORE_GEOMETRY_EXPORTER_H_

#include <ostream>

#include "geometry.h"

namespace selector
{
template<class G>
class GeometryExporter
{
protected:
	/// Convenience alias for the Geometry
	using Geometry = G;
	/// Convenience alias for a shared pointer to a Geometry
	using GeometryPtr = std::shared_ptr<Geometry>;
	/// Convenience alias for a Geometry face Index
	using IndexType = typename Geometry::Index_t;
	/// Convenience alias for a Geometry position
	using PositionType = typename Geometry::PositionType;

public:
	GeometryExporter(GeometryPtr geom) : m_geometry(geom) {}

	/**
	 * Exports the Geometry to the \p outStream.
	 */
	bool Export(std::ostream &outStream)
	{
        std::unordered_map<typename Geometry::Index_t, std::size_t> geometryToExportMap;
        std::size_t pointIndex = 0;

        StartGeometry(outStream);

        for (auto iter = m_geometry->SplitPointsBegin(); iter != m_geometry->SplitPointsEnd(); ++iter)
        {
            auto pointHandle = m_geometry->GetPoint(*iter);
            if (geometryToExportMap.find(pointHandle) == std::end(geometryToExportMap))
            {
                geometryToExportMap[pointHandle] = pointIndex++;
                ExportPoint(outStream, m_geometry->GetPosition(m_geometry->GetPoint(*iter)));
            }
        }

        for (auto faceIter = m_geometry->FacesBegin(); faceIter != m_geometry->FacesEnd(); ++faceIter)
        {
            StartFace(outStream);
            auto circ = m_geometry->FaceSplitPointCirculatorBegin(*faceIter);
            while (circ)
            {
                FaceAddIndex(outStream, geometryToExportMap[m_geometry->GetPoint(*circ)]);
                ++circ;
            }
            EndFace(outStream);
        }
		return true;
	}

protected:
	virtual void StartGeometry(std::ostream &outStream) = 0;
	virtual void ExportPoint(std::ostream &outStream, const PositionType &position) = 0;
	virtual void StartFace(std::ostream &outStream) = 0;
	virtual void FaceAddIndex(std::ostream &outStream, const IndexType &index) = 0;
	virtual void EndFace(std::ostream &outStream) = 0;
	virtual void EndGeometry(std::ostream &outStream) = 0;

	GeometryPtr m_geometry;
};  // class GeometryExporter

/**
 * Class to export geometries in the Obj format.
 */
template<class G>
class ObjExporter : public GeometryExporter<G>
{
public:
	/**
	 * Constructs the GeometryExporter with the given Geometry.
	 */
	explicit ObjExporter(typename GeometryExporter<G>::GeometryPtr geom) : GeometryExporter<G>(geom) {}

protected:
	void StartGeometry(std::ostream &outStream) final {}

	void ExportPoint(std::ostream &outStream, const typename GeometryExporter<G>::PositionType &position) final
	{
		outStream << "v " << position[0] << " " << position[1] << " " << position[2] << "\n";
	}

	void StartFace(std::ostream &outStream) final { outStream << "f "; }

	void FaceAddIndex(std::ostream &outStream, const typename GeometryExporter<G>::IndexType &index) final
	{
		outStream << (index + 1) << " ";
	}

	void EndFace(std::ostream &outStream) final { outStream << "\n"; }

	void EndGeometry(std::ostream &outStream) final {}

};  // class ObjExporter

}  // namespace selector
#endif
