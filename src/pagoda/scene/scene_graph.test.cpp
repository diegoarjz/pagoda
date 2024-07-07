#include "scene_graph.h"
#include "scene_node.h"

#include <gtest/gtest.h>

using namespace pagoda::scene;

class SceneGraphTest : public ::testing::Test {
public:
  void SetUp() override {
    //
    m_sceneGraph = std::make_shared<SceneGraph>();
  }

  void TearDown() override {
    //
  }

protected:
  SceneGraphPtr m_sceneGraph;
};

TEST_F(SceneGraphTest, root_node_is_created_by_default) {
  ASSERT_NE(m_sceneGraph->GetRootNode(), nullptr);
  EXPECT_EQ(m_sceneGraph->GetRootNode()->GetChildCount(), 0);
  EXPECT_EQ(m_sceneGraph->GetRootNode()->GetFullPath().ToString(), "/");
}

TEST_F(SceneGraphTest, node_creation) {
  auto node = m_sceneGraph->CreateNode<SceneNode>(Path{"node"});
  EXPECT_EQ(m_sceneGraph->GetRootNode()->GetChildCount(), 1);
  EXPECT_EQ(node->GetName(), Path("node"));
  std::size_t visited = 0;
  m_sceneGraph->GetRootNode()->ForEachChild(
      [&node, &visited](const SceneNodePtr &n) {
        EXPECT_EQ(n, node);
        ++visited;
        return true;
      });
  EXPECT_EQ(visited, 1);

  EXPECT_EQ(node->GetName(), Path("node"));
  EXPECT_EQ(node->GetFullPath(), Path("/node"));
  EXPECT_EQ(node->GetSceneGraph(), m_sceneGraph);
}

TEST_F(SceneGraphTest, node_parenting) {
  auto parent = m_sceneGraph->CreateNode<SceneNode>(Path{"parent"});
  auto child = m_sceneGraph->CreateNode<SceneNode>(Path{"child"});
  child->SetParent(parent);

  EXPECT_EQ(parent->GetChildCount(), 1);

  std::size_t visited = 0;
  parent->ForEachChild([&child, &visited](const SceneNodePtr &n) {
    EXPECT_EQ(n, child);
    ++visited;
    return true;
  });
  EXPECT_EQ(visited, 1);
  EXPECT_EQ(child->GetParent(), parent);

  EXPECT_EQ(child->GetFullPath(), Path("/parent/child"));

  child->SetParent(nullptr);
  EXPECT_EQ(child->GetParent(), m_sceneGraph->GetRootNode());
  EXPECT_EQ(child->GetFullPath(), Path("/child"));
}

TEST_F(SceneGraphTest, node_path_update)
{
  auto parent = m_sceneGraph->CreateNode<SceneNode>(Path{"parent"});
  auto child1 = m_sceneGraph->CreateNode<SceneNode>(Path{"child1"});
  auto child2 = m_sceneGraph->CreateNode<SceneNode>(Path{"child2"});
  auto child3 = m_sceneGraph->CreateNode<SceneNode>(Path{"child3"});
  child1->SetParent(parent);
  child2->SetParent(parent);
  child3->SetParent(child2);

  EXPECT_EQ(child1->GetFullPath(), Path{"/parent/child1"});
  EXPECT_EQ(child2->GetFullPath(), Path{"/parent/child2"});
  EXPECT_EQ(child3->GetFullPath(), Path{"/parent/child2/child3"});

  child2->SetParent(child1);
  EXPECT_EQ(child2->GetFullPath(), Path{"/parent/child1/child2"});
  EXPECT_EQ(child3->GetFullPath(), Path{"/parent/child1/child2/child3"});

  child2->SetParent(parent);
  EXPECT_EQ(child2->GetFullPath(), Path{"/parent/child2"});
  EXPECT_EQ(child3->GetFullPath(), Path{"/parent/child2/child3"});

  child2->SetParent(nullptr);
  EXPECT_EQ(child2->GetFullPath(), Path{"/child2"});
  EXPECT_EQ(child3->GetFullPath(), Path{"/child2/child3"});
}

TEST_F(SceneGraphTest, node_removal) {
  auto node = m_sceneGraph->CreateNode<SceneNode>(Path{"parent"});

  node->Remove();

  EXPECT_EQ(node->GetParent(), nullptr);
  EXPECT_EQ(node->GetSceneGraph(), nullptr);
  EXPECT_FALSE(node->GetFullPath().IsValid());
  EXPECT_EQ(m_sceneGraph->GetRootNode()->GetChildCount(), 0);
}

class MockSceneGraphListener : public SceneGraphChangeListener {
public:
  void NodeAdded(SceneNodePtr &node) override { added = node; }
  void NodeRemoved(SceneNodePtr &node) override { removed = node; }
  void NodeReparented(SceneNodePtr &node, const SceneNodePtr &oldParent,
                      const SceneNodePtr &newParent) override {
    reparented = node;
    this->oldParent = oldParent;
    this->newParent = newParent;
  }

  SceneNodePtr added{nullptr};
  SceneNodePtr removed{nullptr};

  SceneNodePtr reparented{nullptr};
  SceneNodePtr oldParent{nullptr};
  SceneNodePtr newParent{nullptr};
};

TEST_F(SceneGraphTest, node_created_notification) {
  MockSceneGraphListener l;
  m_sceneGraph->SetSceneGraphListener(&l);

  auto node = m_sceneGraph->CreateNode<SceneNode>(Path{"node"});
  EXPECT_EQ(node, l.added);
}

TEST_F(SceneGraphTest, node_removed_notification) {
  MockSceneGraphListener l;
  m_sceneGraph->SetSceneGraphListener(&l);

  auto node = m_sceneGraph->CreateNode<SceneNode>(Path{"node"});
  EXPECT_EQ(l.removed, nullptr);
  node->Remove();
  EXPECT_EQ(node, l.removed);
}

TEST_F(SceneGraphTest, node_reparent_notification) {
  MockSceneGraphListener l;
  m_sceneGraph->SetSceneGraphListener(&l);

  auto rootNode = m_sceneGraph->GetRootNode();
  auto parent = m_sceneGraph->CreateNode<SceneNode>(Path{"parent"});
  auto child = m_sceneGraph->CreateNode<SceneNode>(Path{"child"});

  EXPECT_EQ(l.reparented, nullptr);
  EXPECT_EQ(l.oldParent, nullptr);
  EXPECT_EQ(l.newParent, nullptr);

  child->SetParent(parent);

  EXPECT_EQ(l.reparented, child);
  EXPECT_EQ(l.oldParent, rootNode);
  EXPECT_EQ(l.newParent, parent);

  child->SetParent(nullptr);

  EXPECT_EQ(l.reparented, child);
  EXPECT_EQ(l.oldParent, parent);
  EXPECT_EQ(l.newParent, rootNode);
}
