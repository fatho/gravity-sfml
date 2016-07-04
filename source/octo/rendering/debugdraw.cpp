#include "debugdraw.hpp"

using namespace octo::rendering;

DrawBuilder<sf::CircleShape> DebugDraw::circle(const sf::Vector2f& center, float radius) {
  DrawBuilder<sf::CircleShape> builder;
  builder.shape().setPosition(center.x - radius, center.y - radius);
  builder.shape().setRadius(radius);
  return builder;
}

DrawBuilder<sf::RectangleShape> DebugDraw::rectangle(const sf::FloatRect& rect) {
  DrawBuilder<sf::RectangleShape> builder;
  builder.shape().setPosition(rect.left, rect.top);
  builder.shape().setSize(sf::Vector2f(rect.width, rect.height));
  return builder;
}

DrawBuilder<sf::RectangleShape> DebugDraw::rectangle(const sf::Vector2f& position,
                                                     const sf::Vector2f& origin,
                                                     const sf::Vector2f& size, float rotation) {
  DrawBuilder<sf::RectangleShape> builder;
  builder.shape().setPosition(position);
  builder.shape().setOrigin(origin);
  builder.shape().setSize(size);
  builder.shape().setRotation(rotation);
  return builder;
}
