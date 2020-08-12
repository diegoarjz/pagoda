#include "geojson_reader.h"
#include "pagoda/math/vec_base.h"

#include <pagoda/common/exception/exception.h>
#include <optional>

#include <pagoda/dynamic/boolean_value.h>
#include <pagoda/dynamic/float_value.h>
#include <pagoda/dynamic/string_value.h>

#include <WGS84toCartesian.hpp>

#include <boost/algorithm/string.hpp>

#include <memory>
#include <unordered_map>
#include <vector>

using namespace pagoda::common::exception;
using namespace pagoda::dynamic;

namespace pagoda::geometry::io
{
GeoJsonReader::GeoJsonReader(const nlohmann::json &json) : m_json(json) {}
GeoJsonReader::GeoJsonReader(const std::string &json) : m_json(nlohmann::json::parse(json)) {}

namespace
{
GeoJsonReader::Feature ReadFeature(const nlohmann::json &json, const std::optional<math::Vec2F> &refCoordinate)
{
	std::string geometryType = json["geometry"]["type"];
	if (boost::to_lower_copy(geometryType) != "polygon")
	{
		throw Exception("Unsupported GeoJSON geometry type " + geometryType);
	}

	auto coordinates = json["geometry"]["coordinates"];
	std::vector<math::Vec2F> points;
	for (const auto &c : coordinates)
	{
		if (c[0].is_array())
		{
			for (auto coord : c)
			{
				points.emplace_back(math::Vec2F{coord[0], coord[1]});
			}
		}
		else
		{
			points.emplace_back(math::Vec2F{c[0], c[1]});
		}
	}

	if (refCoordinate.has_value())
	{
		auto ref = refCoordinate.value();
		for (auto &p : points)
		{
			auto cartesianCoordinate = wgs84::toCartesian({X(ref), Y(ref)}, {X(p), Y(p)});
			p = math::Vec2F{static_cast<float>(cartesianCoordinate[0]), static_cast<float>(cartesianCoordinate[1])};
		}
	}

	if (points.back() == points.front())
	{
		points.pop_back();
	}

	// Clean up
	for (auto i = 0u; i < points.size(); ++i)
	{
		if (points[i] == points[(i + 1) % points.size()])
		{
			points.erase(points.begin() + i);
		}
	}

	std::unordered_map<std::string, DynamicValueBasePtr> properties;
	if (json.contains("properties"))
	{
		for (const auto &[key, value] : json["properties"].items())
		{
			if (value.is_string())
			{
				properties.emplace(key, std::make_shared<String>(value.get<std::string>()));
			}
			else if (value.is_number())
			{
				properties.emplace(key, std::make_shared<FloatValue>(value.get<float>()));
			}
			else if (value.is_boolean())
			{
				properties.emplace(key, std::make_shared<Boolean>(value.get<bool>()));
			}
		}
	}

	core::Polygon2 polygon(std::move(points));

	return GeoJsonReader::Feature{std::move(polygon), std::move(properties)};
}
}  // namespace

void GeoJsonReader::Read(const std::function<bool(Feature &&)> &f)
{
	auto featureType = boost::to_lower_copy(m_json["type"].get<std::string>());
	if (featureType == "feature")
	{
		f(ReadFeature(m_json, m_referenceCoordinate));
	}
	else if (featureType == "featurecollection")
	{
		for (const auto &feature : m_json["features"])
		{
			f(ReadFeature(feature, m_referenceCoordinate));
		}
	}
}

void GeoJsonReader::SetReferenceCoordinate(const math::Vec2F &coordinate) { m_referenceCoordinate = coordinate; }
}  // namespace pagoda::geometry::io
