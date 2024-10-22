#include "ingamestate.hpp"

#include "../game.hpp"
#include "../game/components.hpp"
#include "../rendering/debugdraw.hpp"
#include <octo/math/all.hpp>
#include <octo/game/collision/mask.hpp>
#include <octo/game/collision/util.hpp>

#include <cmath>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <boost/math/constants/constants.hpp>

using namespace octo::states;
using namespace octo::game::components;

InGameState::InGameState() {
  // TODO: take world as an argument
  m_world = std::make_unique<game::World>();
  m_world->setClipRadius(900);

  m_world->addPlanet({300, 200}, 128, 30000);
  m_world->addPlanet({-150, -200}, 128, 30000);
  m_world->addPlanet({150, -200}, 22, 3000);

  for(float angle = 0; angle < 360; angle += 36) {
    float rad = angle / 180.f * boost::math::constants::pi<float>();
    sf::Vector2f dir { std::cos(rad), std::sin(rad) };
    m_world->spawnDebugBullet(sf::Vector2f {-320, -320} + dir * 15.f, dir * 120.f);
  }
  m_paused = true;

  m_world->spawnVessel({450, 250}, 180);
  //auto b = m_world->spawnDebugBullet({300, 60}, {0, 0});

  // m_world->addPlanet({0, 0}, 128, 30000);
  // m_world->spawnDebugBullet({0,-400}, {0, 0});
}

void InGameState::update(sf::Time elapsed) {
  if(m_paused) {
    return;
  }
  // fill time accumulator
  double elapsedSeconds = elapsed.asSeconds();
  if (elapsedSeconds > 0.125) {
    // cap frame rate at 8 FPS to prevent spiral of death.
    // note that this will cause the simulation to run slower
    elapsedSeconds = 0.125f;
  }
  m_timeAccumulator += elapsedSeconds * m_timeFactor;

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
    case sf::Event::KeyPressed:
      switch (event.key.code) {
      case sf::Keyboard::Space:
        m_paused = !m_paused;
        break;
      case sf::Keyboard::LShift:
        m_timeFactor = 0.4f;
        log.debug("bullet time activated");
        break;
      default:
        break;
      }
      break;
    case sf::Event::KeyReleased:
      switch (event.key.code) {
      case sf::Keyboard::LShift:
        m_timeFactor = 1.0f;
        log.debug("bullet time deactivated");
        break;
      default:
        break;
      }
      break;
    default:
      break;
    }
  }
}

void InGameState::activated() {}

void InGameState::draw(sf::RenderTarget& target) {
  applyView(target);
  target.clear(sf::Color::Black);
  drawBackground(target);
  drawPlanets(target);
  debugDraw(target);
}

void InGameState::drawBackground(sf::RenderTarget& target) const {
  // TODO: draw some nice starfield background
  // TODO: draw dangerous looking nebula outside/around world boundary instead of ugly circle
}

void InGameState::drawPlanets(sf::RenderTarget& target) const {
  m_world->entities.each<Spatial, Planet>([&](entityx::Entity, Spatial& spatial, Planet& planet) {
    const SpatialSnapshot& interpolated = spatial.interpolated();
    // TODO: draw planet textures
  });
}

void InGameState::debugDraw(sf::RenderTarget& target) const {
  using rendering::DebugDraw;
  DebugDraw::circle(sf::Vector2f(), m_world->clipRadius()).outline(2, sf::Color::Red).draw(target);
  m_world->entities.each<Spatial>([&](entityx::Entity e, Spatial& spatial) {
    const SpatialSnapshot& interpolated = spatial.interpolated();
    auto debugData = e.component<DebugData>();
    auto coll = e.component<CollisionMask>();
    auto body = e.component<DynamicBody>();
    // show collision mask
    if (debugData && coll) {
      const sf::Texture& tex = debugData->collisionMaskTexture;
      sf::Sprite sprite(tex);
      sprite.setOrigin(math::vector::vector_cast<float>(tex.getSize()) * 0.5f - coll->anchor);
      sprite.setRotation(interpolated.rotationDegrees);
      sprite.setPosition(interpolated.position);
      target.draw(sprite);
    }
    // show rotation
    DebugDraw::rectangle(interpolated.position, {0.5, 16}, {1 * m_viewZoom, 16}, interpolated.rotationDegrees)
        .fill(sf::Color::Red)
        .draw(target);
    // show velocity
    if (body) {
      auto vel = body->velocity();
      float mag = math::vector::length(vel);
      DebugDraw::rectangle(interpolated.position,
                           {0, 0.5},
                           {mag, 1 * m_viewZoom},
                           std::atan2(vel.y, vel.x) * 180.f / boost::math::constants::pi<float>())
          .fill(sf::Color::Green)
          .draw(target);
    }
  });
}

void InGameState::applyView(sf::RenderTarget& target) const {
  sf::View view;
  view.setCenter(m_viewCenter);
  view.setSize(math::vector::vector_cast<float>(target.getSize()) * m_viewZoom);
  target.setView(view);
}
