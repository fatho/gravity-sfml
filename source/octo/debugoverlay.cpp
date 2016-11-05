#include "debugoverlay.hpp"

#include <octo/math/vector.hpp>
#include <octo/math/rect.hpp>

#include <SFML/Graphics.hpp>
#include <boost/format.hpp>

namespace octo {

void DebugOverlay::draw(sf::RenderTarget& target) {
  if (m_active && m_font) {
    sf::View realView(sf::FloatRect({0,0}, math::vector::vector_cast<float>(target.getSize())));
    target.setView(realView);

    sf::Text fpsText;
    fpsText.setFont(m_font->content());
    fpsText.setCharacterSize(16);
    fpsText.setString((boost::format("FPS: %.1f") % m_fps.fps()).str());
    fpsText.setFillColor(sf::Color::White);
    fpsText.setPosition(5, 5);
    target.draw(fpsText);
  }
}

void DebugOverlay::update(sf::Time) {
  m_fps.nextFrame();
}

bool DebugOverlay::active() const {
  return m_active;
}

void DebugOverlay::setActive(bool active) {
  m_active = active;
}

const std::shared_ptr<content::FontContent>& DebugOverlay::font() const {
  return m_font;
}

void DebugOverlay::setFont(std::shared_ptr<content::FontContent> font) {
  m_font = std::move(font);
}
}
