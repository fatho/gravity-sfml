#pragma once

#include "../game/world.hpp"
#include "../gamestate.hpp"

#include <memory>
#include <SFML/Graphics/View.hpp>

namespace octo {
namespace states {

class InGameState : public GameState {
public:
  InGameState();

  void update(sf::Time elapsed) override;
  void handleEvents() override;

protected:
  void draw(sf::RenderTarget& target) override;
  void drawBackground(sf::RenderTarget& target) const;
  void drawPlanets(sf::RenderTarget& target) const;
  void debugDraw(sf::RenderTarget& target) const;

  void activated() override;

  void applyView(sf::RenderTarget& target) const;

private:
  std::unique_ptr<game::World> m_world;

  sf::Vector2f m_viewCenter;
  float m_viewZoom = 1.0f;

  float m_physicsStep = 1.f / 60.f;
  float m_timeAccumulator = 0;
  int m_maxStepsPerFrame = 10;
};

}
}
