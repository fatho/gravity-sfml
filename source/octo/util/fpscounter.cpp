#include "fpscounter.hpp"

namespace octo {
namespace util {

void FpsCounter::nextFrame() {
  float elapsed = m_updateClock.getElapsedTime().asSeconds();
  if(elapsed >= m_updateIntervalSec) {
    m_fps = m_frameCount / elapsed;
    m_frameCount = 0;
    m_updateClock.restart();
  }
  m_frameCount += 1;
}

float FpsCounter::fps() const {
  return m_fps;
}

}
}
