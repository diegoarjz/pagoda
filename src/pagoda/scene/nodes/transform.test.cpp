#include "transform.h"
#include "pagoda/scene/scene_graph.h"

#include <gtest/gtest.h>

using namespace pagoda::scene;

TEST(Transform, test_construction) {
  auto sceneGraph = std::make_shared<SceneGraph>();
  auto t = sceneGraph->CreateNode<nodes::Transform>(Path{"t1"});

  ASSERT_NE(t, nullptr);

  EXPECT_TRUE(t->IsDirty());
  EXPECT_EQ(t->GetTransform(), Transformation{});
}
