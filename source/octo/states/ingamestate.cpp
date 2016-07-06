#include "ingamestate.hpp"

#include "../game.hpp"
#include "../game/components.hpp"
#include "../rendering/debugdraw.hpp"
#include <more-math/all.hpp>

#include <iostream>

#include <SFML/Graphics.hpp>

using namespace octo::states;
using namespace octo::game::components;

InGameState::InGameState() {
  // TODO: take world as an argument
  m_world = std::make_unique<game::World>();
  m_world->setClipRadius(500);

  m_world->addPlanet({150, 100}, 50, 10000);
  m_world->addPlanet({-75, -100}, 50, 10000);
  m_world->spawnDebugBullet({-150, -150}, {80, -70});

  // m_world->addPlanet({0, 0}, 128, 30000);
  // m_world->spawnDebugBullet({0,-400}, {0, 0});

  // initial view shows everything
  float worldRadius = m_world->clipRadius();
  m_view.reset(sf::FloatRect(-worldRadius, -worldRadius, 2 * worldRadius, 2 * worldRadius));
}

void InGameState::update(sf::Time elapsed) {
  // fill time accumulator
  double elapsedSeconds = elapsed.asSeconds();
  if (elapsedSeconds > 0.125) {
    // cap frame rate at 8 FPS to prevent spiral of death.
    // note that this will cause the simulation to run slower
    elapsedSeconds = 0.125f;
  }
  m_timeAccumulator += elapsedSeconds;

  // perform fixed time steps on accumulated time
  while (m_timeAccumulator >= m_physicsStep) {
    m_world->update(m_physicsStep);
    m_timeAccumulator -= m_physicsStep;
  }

  // interpolation
  const double alpha = m_timeAccumulator / m_physicsStep;
  m_world->interpolateState(alpha);
}

void InGameState::handleEvents() {
  sf::RenderWindow& window = game()->window();
  sf::Event event;
  while (window.pollEvent(event)) {
    switch (event.type) {
    case sf::Event::Closed:
      // TODO: ask user or save state before exiting
      window.close();
      break;
    case sf::Event::Resized:
      this->rebuildView();
      // TODO: update "view" after resizing
      break;
    default:
      break;
    }
  }
}

void InGameState::activated() {
  this->rebuildView();
}

void InGameState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.clear(sf::Color::Black);
  drawBackground(target, states);
  drawPlanets(target, states);
  debugDraw(target, states);
}

void InGameState::drawBackground(sf::RenderTarget& target, sf::RenderStates states) const {
  // TODO: draw some nice starfield background
  // TODO: draw dangerous looking nebula outside/around world boundary instead of ugly circle
}

void InGameState::drawPlanets(sf::RenderTarget& target, sf::RenderStates states) const {
  m_world->entities().each<Spatial, Planet>([&](entityx::Entity, Spatial& spatial, Planet& planet) {
    const SpatialSnapshot& interpolated = spatial.interpolated();
    sf::Sprite planetSprite(planet.terrainTexture);
    planetSprite.setOrigin(planet.size() * 0.5f);
    planetSprite.setPosition(interpolated.position);
    planetSprite.setRotation(interpolated.rotationDegrees);
    target.draw(planetSprite, states);
  });
}

void InGameState::debugDraw(sf::RenderTarget& target, sf::RenderStates states) const {
  using rendering::DebugDraw;
  DebugDraw::circle(sf::Vector2f(), m_world->clipRadius())
      .outline(2, sf::Color::Red)
      .draw(target, states);

  m_world->entities().each<Spatial>([&](entityx::Entity e, Spatial& spatial) {
    const SpatialSnapshot& interpolated = spatial.interpolated();
    auto planet = e.component<Planet>();
    auto body = e.component<DynamicBody>();
    if (planet) {
      DebugDraw::rectangle(
          math::rect::fromCenterSize(spatial.interpolated().position, planet->size()))
          .outline(2, sf::Color::Blue)
          .draw(target, states);
    }
    if (body) {
      DebugDraw::circle(interpolated.position, 0.5).outline(2, sf::Color::Red).draw(target, states);
      DebugDraw::rectangle(interpolated.position, {2, 16}, {4, 16}, interpolated.rotationDegrees)
          .fill(sf::Color::Red)
          .draw(target, states);
    }
  });
}

void InGameState::rebuildView() {
  sf::Vector2f center = m_view.getCenter(); // save old center
  sf::Vector2u size = game()->window().getSize();
  m_view.reset(sf::FloatRect(0, 0, size.x, size.y));
  m_view.setCenter(center); // restore old center
  game()->window().setView(m_view);
}
