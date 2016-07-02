#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

namespace octo {
namespace rendering {

/*! \brief Provides an interface for constructing SFML shapes that allows chaining.
 */
template<class ShapeType>
class DrawBuilder {
public:
  /*! \brief The shape constructed by this instance.
   *  \returns the shape under construction.
   */
  ShapeType& shape() {
    return m_shape;
  }

  /*! \brief Updates the shape's fill color.
   *  \param color the new fill color.
   *  \returns a reference to this instance for chaining.
   */
  DrawBuilder<ShapeType>& fill(sf::Color color) {
    m_shape.setFillColor(color);
    return *this;
  }

  /*! \brief Updates the shape's outline.
   *  \param thickness the oulines thickness, will be multiplied with the thickness factor of this builder.
   *  \param color the new outline color.
   *  \returns a reference to this instance for chaining.
   */
  DrawBuilder<ShapeType>& outline(float thickness, sf::Color color) {
    m_shape.setOutlineThickness(thickness);
    m_shape.setOutlineColor(color);
    return *this;
  }

  /*! \brief Renders the shape.
   *
   *  This builder is valid after finishing the draw and can be used for further renderings.
   *  \param target the render target
   *  \param states current render states
   *  \returns a reference to this instance for chaining.
   */
  DrawBuilder<ShapeType>& draw(sf::RenderTarget& target, const sf::RenderStates& states) {
    target.draw(m_shape, states);
    return *this;
  }

private:
  /// the shape under construction
  ShapeType m_shape;
};

/*! \brief A simple interface for drawing SFML's primitive shapes.
 */
class DebugDraw {
public:
  /*! \brief Starts constructing a circle.
   *  \param center the circle's center point.
   *  \param radius the circle's radius.
   *  \return a DrawBuilder for setting the visual properties of the sf::CircleShape.
   */
  static DrawBuilder<sf::CircleShape> circle(const sf::Vector2f& center, float radius);

  /*! \brief Starts constructing a rectangle.
   *  \param rect the rectangle.
   *  \return a DrawBuilder for setting the visual properties of the sf::RectangleShape.
   */
  static DrawBuilder<sf::RectangleShape> rectangle(const sf::FloatRect& rect);
};

}
}
