#include "path.h"

#include <gtest/gtest.h>

using namespace pagoda::scene;

TEST(PathTest, construction) {
  std::string path = "/abc/def/ghi";
  Path p{path};

  EXPECT_TRUE(p.IsValid());
  EXPECT_TRUE(p.IsAbsolute());
  EXPECT_EQ(p.ToString(), path);
  EXPECT_EQ(p.GetNumComponents(), 3);

  path = "abc/def/ghi";
  p = Path{path};
  EXPECT_TRUE(p.IsValid());
  EXPECT_FALSE(p.IsAbsolute());
  EXPECT_EQ(p.ToString(), path);
  EXPECT_EQ(p.GetNumComponents(), 3);

  EXPECT_EQ(Path{"/"}.ToString(), "/");
  EXPECT_TRUE(Path{"/"}.IsValid());
}

TEST(PathTest, parent) {
  std::string path = "/abc/def/ghi";
  Path p{path};

  EXPECT_TRUE(p.IsValid());
  EXPECT_EQ(p.GetParent().ToString(), "/abc/def");

  path = "abc/def/ghi";
  p = Path{path};

  EXPECT_TRUE(p.IsValid());
  EXPECT_EQ(p.GetParent().ToString(), "abc/def");
}

TEST(PathTest, append_child) {
  Path p{"abc/def"};
  EXPECT_EQ(p.AppendChild(Path{"ghi"}).ToString(), "abc/def/ghi");
  EXPECT_FALSE(p.AppendChild(Path{"ghi/jkl"}).IsValid());
}

TEST(PathTest, append_path) {
  Path p{"abc"};
  EXPECT_EQ(p.AppendPath(Path{"def"}).ToString(), "abc/def");
}

TEST(PathTest, name) {
  EXPECT_EQ(Path{"abc"}.GetName().ToString(), "abc");
  EXPECT_EQ(Path{"abc/def"}.GetName().ToString(), "def");
}

TEST(PathTest, replace_name) {
  EXPECT_EQ(Path{"abc"}.ReplaceName(Path{"def"}).ToString(), "def");
  EXPECT_EQ(Path{"abc/def"}.ReplaceName(Path{"ghi"}).ToString(), "abc/ghi");
}

TEST(PathTest, is_root) {
  EXPECT_TRUE(Path{"/"}.IsRoot());
  EXPECT_FALSE(Path{"/abc"}.IsRoot());
  EXPECT_FALSE(Path{"abc"}.IsRoot());
}

TEST(PathTest, has_prefix) {
  Path p1{"/abc/def/ghi"};
  Path p2{"abc/def/ghi"};

  EXPECT_TRUE(p1.HasPrefix(Path{"/abc/def"}));
  EXPECT_TRUE(p1.HasPrefix(Path{"/abc"}));
  EXPECT_FALSE(p1.HasPrefix(Path{"abc/def"}));
  EXPECT_FALSE(p1.HasPrefix(Path{"abc"}));
  EXPECT_FALSE(p1.HasPrefix(Path{"xbc/def"}));
  EXPECT_FALSE(p1.HasPrefix(Path{"xbc"}));

  EXPECT_TRUE(p2.HasPrefix(Path{"abc/def"}));
  EXPECT_TRUE(p2.HasPrefix(Path{"abc"}));
  EXPECT_FALSE(p2.HasPrefix(Path{"/abc/def"}));
  EXPECT_FALSE(p2.HasPrefix(Path{"/abc"}));
  EXPECT_FALSE(p2.HasPrefix(Path{"/xbc/def"}));
  EXPECT_FALSE(p2.HasPrefix(Path{"/xbc"}));
}

TEST(PathTest, invalid_paths) {
  EXPECT_FALSE(Path{"/a b c"}.IsValid());
  EXPECT_FALSE(Path{"d e f"}.IsValid());
  EXPECT_FALSE(Path{"a~~~~~"}.IsValid());
}
