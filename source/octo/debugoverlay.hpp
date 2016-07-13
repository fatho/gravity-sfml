#pragma once

#include <octo/util/fpscounter.hpp>
#include <octo/content/font.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

#include <memory>

namespace octo {

class DebugOverlay {
public:

  void draw(sf::RenderTarget& target);

  void update(sf::Time elapsed);

  bool active() const;

  void setActive(bool active);

  const std::shared_ptr<content::FontContent>& font() const;

  void setFont(std::shared_ptr<content::FontContent> font);

private:
  std::shared_ptr<content::FontContent> m_font;
  util::FpsCounter m_fps;
  bool m_active = true;
};

}
