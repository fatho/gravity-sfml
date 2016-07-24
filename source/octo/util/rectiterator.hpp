#pragma once

#include <SFML/Graphics/Rect.hpp>

namespace octo {
namespace util {

template <class T>
class RowMajorIterator {
public:
  RowMajorIterator(const sf::Rect<T>& rect)
      : m_rect(rect), m_current(m_rect.left, m_rect.top + m_rect.height) {}

  RowMajorIterator(const sf::Rect<T>& rect, const sf::Vector2<T>& initial) : m_rect(rect) {
    if (m_rect.contains(initial)) {
      m_current = initial;
    } else {
      m_current = {m_rect.left, m_rect.top + m_rect.height};
    }
  }

  const sf::Vector2<T>& operator*() {
    return m_current;
  }

  RowMajorIterator<T>& operator++() {
    if (m_rect.contains(m_current)) {
      m_current.x += 1;
      if (m_current.x >= m_rect.left + m_rect.width) {
        m_current.x = m_rect.left;
        m_current.y += 1;
      }
    }
    return *this;
  }

  bool operator==(const RowMajorIterator<T>& other) {
    return this->m_rect == other.m_rect && this->m_current == other.m_current;
  }

  bool operator!=(const RowMajorIterator<T>& other) {
    return !(*this == other);
  }

private:
  const sf::Rect<T>& m_rect;
  sf::Vector2<T> m_current;
};

template <class T, class IteratorStrategy = RowMajorIterator<T>>
class RectRange {
public:
  explicit RectRange(const sf::Rect<T>& rect) : m_rect(rect) {}

  IteratorStrategy begin() {
    return {m_rect, {m_rect.left, m_rect.top}};
  }

  IteratorStrategy end() {
    return {m_rect};
  }

private:
  const sf::Rect<T>& m_rect;
};

template <class T, class IteratorStrategy = RowMajorIterator<T>>
RectRange<T, IteratorStrategy> rectRange(const sf::Rect<T>& rect) {
  return RectRange<T, IteratorStrategy>(rect);
}
}
}
