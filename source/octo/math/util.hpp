#pragma once

#include <boost/math/constants/constants.hpp>

namespace octo {
namespace math {
namespace util {

template<class T>
inline constexpr T sqr(T value) {
  return value * value;
}

inline float degToRad(float degrees) {
  return degrees / 180.f * boost::math::constants::pi<float>();
}

}
}
}
