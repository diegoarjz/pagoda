#include "delegate.h"

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::common;

TEST(DelegateTest, can_add_std_functions) {
  bool called = false;
  const auto fn = [&]() {called = true;};
  Delegate<void> d;
  d.AddCallback(fn);
  d();
  EXPECT_TRUE(called) << "Callback should have been called";
}

TEST(DelegateTest, can_add_multiple_std_functions) {
  int called = 0;
  const auto fn = [&]() {++called;};
  const auto fn2 = [&]() {++called;};
  Delegate<void> d;
  d.AddCallback(fn);
  d.AddCallback(fn2);
  d();
  EXPECT_EQ(called, 2) << "Callback should have been called twice";
}

TEST(DelegateTest, can_remove_std_functions) {
  bool called = false;
  const auto fn = [&]() {called = true;};
  Delegate<void> d;
  auto handler = d.AddCallback(fn);
  d.RemoveCallback(handler);
  d();
  EXPECT_FALSE(called) << "Callback should not have been called";
}

static bool called = false;
void callback() { called = true; }
TEST(DelegateTest, can_add_free_function) {
  Delegate<void> d;
  d.AddCallback(&callback);
  d();
  EXPECT_TRUE(called);
}

class functor {
public:
  void operator()() {
    called = true;
  }
};

TEST(DelegateTest, can_add_functor_objects) {
  called = false;
  functor f;
  Delegate<void> d;
  d.AddCallback(f);
  d();
  EXPECT_TRUE(called);
}
