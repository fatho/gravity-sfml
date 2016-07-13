#pragma once

#include <SFML/System/Clock.hpp>

namespace octo {
namespace util {

class FpsCounter {
public:
  void nextFrame();
  float fps() const;
private:
  int m_frameCount = 0;
  float m_fps = 0;
  sf::Clock m_updateClock;
  float m_updateIntervalSec = 1;
};


}
}
