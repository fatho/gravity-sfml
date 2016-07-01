#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

namespace octo {
namespace rendering {

template<class ShapeType>
class DrawBuilder {
public:
  DrawBuilder(sf::RenderTarget& target, sf::RenderStates& states)
    : m_target(target), m_states(states) {
    m_shape.setFillColor(sf::Color::Transparent);
    m_shape.setOutlineColor(sf::Color::Transparent);
  }

  ShapeType& shape() {
    return m_shape;
  }

  DrawBuilder<ShapeType>& fill(sf::Color color) {
    m_shape.setFillColor(color);
    return *this;
  }

  DrawBuilder<ShapeType>& outline(float thickness, sf::Color color) {
    m_shape.setOutlineThickness(thickness);
    m_shape.setOutlineColor(color);
    return *this;
  }

  void draw() {
    m_target.draw(m_shape, m_states);
  }

private:
  sf::RenderTarget& m_target;
  sf::RenderStates& m_states;
  ShapeType m_shape;
};

class DebugDraw {
public:
  DebugDraw(sf::RenderTarget& target, sf::RenderStates& states, float thicknessFactor);

  DrawBuilder<sf::CircleShape> circle(const sf::Vector2f& center, float radius);

  DrawBuilder<sf::RectangleShape> rectangle(const sf::FloatRect& rect);

private:
  sf::RenderTarget& m_target;
  sf::RenderStates& m_states;
  float m_thicknessFactor;
};

}
}
