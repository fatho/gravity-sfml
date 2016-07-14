#pragma once

#include <SFML/Graphics/Image.hpp>
#include <boost/range/irange.hpp>

#include <algorithm>
#include <vector>

namespace octo {
namespace util {

template<typename PixelType = sf::Uint32>
class PixelArray {
public:
  using coord_range = boost::integer_range<size_t>;

  PixelArray(size_t width, size_t height, PixelType fill = static_cast<PixelType>(0))
    : m_pixels(width * height, fill), m_width(width), m_height(height)
  {
  }

  PixelArray(PixelArray&& other)
    : m_width(0), m_height(0) {
    swap(*this, other);
  }

  PixelArray& operator=(PixelArray&& other) {
    PixelArray destruct;
    swap(*this, destruct);
    swap(other, *this);
  }

  size_t index(size_t x, size_t y) const {
    return y * m_width + x;
  }

  PixelType& at(size_t x, size_t y) {
    return m_pixels[index(x,y)];
  }

  PixelType at(size_t x, size_t y) const {
    return m_pixels[index(x,y)];
  }

  void clear(PixelType value) {
    std::fill(begin(m_pixels), end(m_pixels), value);
  }

  void resize(size_t newWidth, size_t newHeight) {
    m_pixels.resize(newWidth * newHeight);
    m_width = newWidth;
    m_height = newHeight;
  }

  coord_range xrange() const {
    return boost::irange(static_cast<size_t>(0), m_width - 1);
  }

  coord_range yrange() const {
    return boost::irange(static_cast<size_t>(0), m_height - 1);
  }

  template<typename Converter>
  sf::Image toImage(Converter conv = Converter()) const {
    sf::Image result;
    result.create(m_width, m_height);
    for(size_t y : yrange()) {
      for(size_t x : xrange()) {
        result.setPixel(x, y, conv(at(x,y)));
      }
    }
    return result;
  }

  size_t width() const {
    return m_width;
  }

  size_t height() const {
    return m_height;
  }

  sf::Vector2<size_t> size() const {
    return { m_width, m_height };
  }

private:
  template<typename Pixel>
  friend void swap(PixelArray<Pixel>& a1, PixelArray<Pixel>& a2);

  std::vector<PixelType> m_pixels;
  size_t m_width;
  size_t m_height;
};

template<typename PixelType>
void swap(PixelArray<PixelType>& a1, PixelArray<PixelType>& a2) {
  std::swap(a1.m_pixels, a2.m_pixels);
  std::swap(a1.m_width, a2.m_width);
  std::swap(a1.m_height, a2.m_height);
}

}
}
