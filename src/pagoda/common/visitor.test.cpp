#include <gtest/gtest.h>

#include "visitor.h"

#include <iostream>

using namespace pagoda::common;

class B;

class A : public Visitable<A> {
public:
  void BeenVisited() { visited = true; }

  bool visited{false};
};

class B : public Visitable<B> {
public:
};

class C : public Visitable<C> {
public:
};

class D : public A {
public:
  void D_WasVisited() { visited_d = true; }

  bool visited_d{false};
};

class VisA : public Visitor<A, B, C, D> {
public:
  using Visitor<A, B, C, D>::Visit;

  void Visit(A &visitable) override { visitable.BeenVisited(); }

  void Visit(D &visitable) override { visitable.D_WasVisited(); }
};

TEST(VisitorTest, testA) {
  A a;
  B b;
  C c;
  D d;
  VisA vis;

  vis.Visit(a);
  vis.Visit(b);
  vis.Visit(c);
  vis.Visit(d);

  EXPECT_TRUE(a.visited);
  EXPECT_TRUE(d.visited_d);
}
