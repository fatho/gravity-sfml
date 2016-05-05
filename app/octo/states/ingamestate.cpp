#include "ingamestate.hpp"

#include "../game.hpp"
#include "../game/components.hpp"

#include <iostream>

#include <SFML/Graphics.hpp>

using namespace octo::states;
using namespace octo::game::components;

InGameState::InGameState() {
  m_world = std::make_unique<game::World>();
  m_world->addPlanet({500,500}, 128);
}

void InGameState::update( sf::Time elapsed ) {
  m_world->update(elapsed);
}

void InGameState::handleEvents() {
  sf::RenderWindow& window = game()->window();
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      // TODO: ask user or save state before exiting
      window.close();
    }
  }
}

void InGameState::activated() {
  game()->window().setView(game()->window().getDefaultView());
}

void InGameState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.clear(sf::Color::Black);
  // TODO: draw some nice starfield background
  drawPlanets(target, states);
}

void InGameState::drawPlanets(sf::RenderTarget& target, sf::RenderStates states) const {
  m_world->entities().each<Position, Planet>([&](entityx::Entity, Position& pos, Planet& planet) {
      sf::Sprite planetSprite(planet.terrainTexture);
      planetSprite.setPosition(pos.position - planet.sizef() / 2.f);
      target.draw(planetSprite, states);
    });
}
