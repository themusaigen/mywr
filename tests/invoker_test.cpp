#include <gtest/gtest.h>
#include <string>
#include <string_view>

#include "mywr/mywr.hpp"

using namespace mywr::invoker;

class A {
public:
  static int sum(int a, int b) { return a + b; }
  static std::string to_str(int a) { return std::to_string(a); }
  static std::string transform(std::string_view str) {
    std::string out{str};
    out += " transformed!";
    return out;
  }
};

TEST(InvokerTest, HandlesBasicCalls) {
  EXPECT_EQ(invoke<decltype(&A::sum)>(&A::sum, 1, 1), 2);

  // In .dll code it would be like...
  // invoke<sum_t>(0xDEADBEEF, 1, 1);
}

TEST(InvokerTest, HandlesNonPodTypes) {
  EXPECT_EQ(invoke<decltype(&A::to_str)>(&A::to_str, 123), "123");
  EXPECT_EQ(invoke<decltype(&A::transform)>(&A::transform, "123"),
            "123 transformed!");
}

#if defined(MYWR_WINDOWS)
class B {
public:
  static int MYWR_STDCALL get_doubled(int a) { return a * 2; }
  int return_len_of_str(std::string_view str) { return str.size(); }
  static std::string MYWR_FASTCALL transform(std::string_view str) {
    std::string out{str};
    out += " transformed!";
    return out;
  };
};

TEST(InvokerTest, HandlesNonDefaultCC) {
  using get_doubled_t = decltype(&B::get_doubled);
  using return_len_of_str_t = decltype(&B::return_len_of_str);
  using transform_t = decltype(&B::transform);

  B b;

  EXPECT_EQ(invoke<get_doubled_t>(&B::get_doubled, 2), 4);
  EXPECT_EQ(invoke<return_len_of_str_t>(&B::return_len_of_str, &b, "123"), 3);
  EXPECT_EQ(invoke<transform_t>(&B::transform, "123"), "123 transformed!");
}
#endif
