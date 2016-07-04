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
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  void drawBackground(sf::RenderTarget& target, sf::RenderStates states) const;
  void drawPlanets(sf::RenderTarget& target, sf::RenderStates states) const;
  void debugDraw(sf::RenderTarget& target, sf::RenderStates states) const;

  void activated() override;

  void rebuildView();
private:
  std::unique_ptr<game::World> m_world;
  sf::View m_view;

  float m_physicsStep = 1.f / 60.f;
  float m_timeAccumulator = 0;
  int m_maxStepsPerFrame = 10;
};

}
}
