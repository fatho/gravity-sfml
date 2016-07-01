#include "debugdraw.hpp"

using namespace octo::rendering;

DebugDraw::DebugDraw(sf::RenderTarget& target, sf::RenderStates& states, float thicknessFactor)
  : m_target(target), m_states(states), m_thicknessFactor(thicknessFactor) {
}

DrawBuilder<sf::CircleShape> DebugDraw::circle(const sf::Vector2f& center, float radius) {
  DrawBuilder<sf::CircleShape> builder(m_target, m_states);
  builder.shape().setPosition(center.x - radius, center.y - radius);
  builder.shape().setRadius(radius);
  return builder;
}

DrawBuilder<sf::RectangleShape> DebugDraw::rectangle(const sf::FloatRect& rect) {
  DrawBuilder<sf::RectangleShape> builder(m_target, m_states);
  builder.shape().setPosition(rect.top, rect.left);
  builder.shape().setSize(sf::Vector2f(rect.width, rect.height));
  return builder;
}
