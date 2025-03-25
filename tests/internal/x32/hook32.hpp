#pragma once

struct NonPod {
  int value0{};
  int value1{};
  int value2{};

  friend auto operator==(const NonPod& lhs, const NonPod& rhs) noexcept
      -> bool {
    return lhs.value0 == rhs.value0 && lhs.value1 == rhs.value1 &&
           lhs.value2 == rhs.value2;
  }
};
