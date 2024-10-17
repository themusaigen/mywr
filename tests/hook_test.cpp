#include <gtest/gtest.h>

#include "mywr/mywr.hpp"

using namespace mywr::hook;

struct non_pod {
  int a;
  int b;
  int c;
  int d;

  non_pod(int a, int b, int c, int d)
      : a(a)
      , b(b)
      , c(c)
      , d(d) {}
};

bool operator==(const non_pod& lhs, const non_pod& rhs) {
  return lhs.a == rhs.a && lhs.b == rhs.b && lhs.c == rhs.c && lhs.d == rhs.d;
}

int MYWR_STDCALL sum(int a, int b) {
  return a + b;
}

non_pod ret_non_pod() {
  return {10, 20, 30, 40};
}

TEST(HookTest, HandlesCdecl) {
  hook<decltype(&sum)> hook;
  hook.set(&sum);
  hook.set([](const auto& hook, int a, int b) {
    return a + b;
  });

  hook.install();

  EXPECT_EQ(sum(2, 2), 4);
}

TEST(HookTest, HandlesNonPod) {
  hook<decltype(&ret_non_pod)> hook;
  hook.set(&ret_non_pod);
  hook.set([](const auto& hook) {
    return non_pod{40, 30, 20, 10};
  });

  hook.install();

  non_pod obj{40, 30, 20, 10};

  EXPECT_EQ(ret_non_pod(), obj);
}
