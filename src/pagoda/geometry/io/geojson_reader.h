#pragma once

#include <pagoda/geometry/core/polygon.h>
#include "pagoda/math/vec_base.h"

#include <nlohmann/json.hpp>

#include <memory>
#include <unordered_map>

namespace pagoda::dynamic
{
class DynamicValueBase;
using DynamicValueBasePtr = std::shared_ptr<DynamicValueBase>;
}  // namespace pagoda::dynamic

namespace pagoda::geometry::io
{
/**
 * Class to read GeoJSON, extracting its features.
 */
class GeoJsonReader
{
	public:
	/**
	 * Represents a feature.
	 */
	struct Feature
	{
		core::Polygon2 m_polygon;
		std::unordered_map<std::string, dynamic::DynamicValueBasePtr> m_properties;
	};

	GeoJsonReader(const nlohmann::json &json);
	GeoJsonReader(const std::string &json);

	/**
	 * Reads all GeoJSON features, calling \p f for each.
	 */
	void Read(const std::function<bool(Feature &&)> &f);

	/**
	 * Sets the wgs84 coordinate (latitude, longitude) that will be
	 * converted to the cartesian coordinate at (0,0).
	 */
	void SetReferenceCoordinate(const math::Vec2F &coordinate);

	private:
	nlohmann::json m_json;
	std::optional<math::Vec2F> m_referenceCoordinate;
};
}  // namespace pagoda::geometry::io
