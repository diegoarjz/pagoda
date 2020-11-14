#include <pagoda/dynamic/dynamic_value_base.h>
#include <pagoda/geometry/io/geojson_reader.h>

#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include <array>
#include <iostream>

using namespace pagoda;
using namespace pagoda::geometry;
using namespace pagoda::geometry::core;
using namespace pagoda::geometry::io;

TEST(GeoJsonReaderTest, should_be_able_to_read_a_single_feature)
{
	auto geojson = nlohmann::json::parse(R"(
    {
      "type": "Feature",
      "geometry": {
        "type": "Polygon",
        "coordinates": [[0,0], [1,0], [1,1], [0,1]]
      }
    })");

	GeoJsonReader r(geojson);
	GeoJsonReader::Feature feature;

	r.Read([&](GeoJsonReader::Feature &&f) {
		feature = f;
		return true;
	});

	EXPECT_EQ(feature.m_polygon.GetPointCount(), 4u);
	EXPECT_EQ(feature.m_polygon, Polygon2({{0, 0}, {1, 0}, {1, 1}, {0, 1}}));
}

TEST(GeoJsonReaderTest, should_be_able_to_read_a_feature_collection)
{
	auto geojson = nlohmann::json::parse(R"(
    {
      "type": "FeatureCollection",
      "features": [
        {
          "type": "Feature",
          "geometry": {
            "type": "Polygon",
            "coordinates": [[0,0], [1,0], [1,1], [0,1]]
          }
        },
        {
          "type": "Feature",
          "geometry": {
            "type": "Polygon",
            "coordinates": [[1,1], [2,1], [2,2], [1,2]]
          }
        }
      ]
    })");

	GeoJsonReader r(geojson);
	std::vector<GeoJsonReader::Feature> features;

	r.Read([&](GeoJsonReader::Feature &&f) {
		features.emplace_back(f);
		return true;
	});

	EXPECT_EQ(features.size(), 2u);
	EXPECT_EQ(features[0].m_polygon, Polygon2({{0, 0}, {1, 0}, {1, 1}, {0, 1}}));
	EXPECT_EQ(features[1].m_polygon, Polygon2({{1, 1}, {2, 1}, {2, 2}, {1, 2}}));
}

TEST(GeoJsonReaderTest, should_be_able_to_read_properties)
{
	auto geojson = nlohmann::json::parse(R"(
    {
      "type": "Feature",
      "geometry": {
        "type": "Polygon",
        "coordinates": [[0,0], [1,0], [1,1], [0,1]]
      },
      "properties": {
        "name": "feature",
        "height": 123,
        "b": true
      }
    })");

	GeoJsonReader r(geojson);
	GeoJsonReader::Feature feature;

	r.Read([&](GeoJsonReader::Feature &&f) {
		feature = f;
		return true;
	});

	EXPECT_EQ(feature.m_properties.size(), 3u);
	EXPECT_EQ(feature.m_properties["name"]->ToString(), "feature");
	EXPECT_EQ(feature.m_properties["height"]->ToString(), "123.000000");
	EXPECT_EQ(feature.m_properties["b"]->ToString(), "true");
}
