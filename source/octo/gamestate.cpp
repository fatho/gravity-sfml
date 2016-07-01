#include "gamestate.hpp"
#include "game.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

using namespace octo;

Game* GameState::game() {
  return m_game;
}

void GameState::setGame(Game* game) {
  m_game = game;
}

void GameState::added() {}

void GameState::removed() {}

void GameState::activated() {}

void GameState::deactivated() {}

GameState::~GameState() {}

void NullState::handleEvents() {
  sf::RenderWindow& window = game()->window();
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed)
      window.close();
  }
}

void NullState::update(sf::Time elapsed) {}

void NullState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.clear(sf::Color::Black);
}
