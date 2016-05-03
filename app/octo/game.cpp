#include "game.hpp"
#include "gamestate.hpp"
#include "content/sfml.hpp"

#include <boost/filesystem.hpp>

using namespace octo;

Game::Game() {
  // TODO: load settings

  // setup subsystems
  m_content.setBasePath(boost::filesystem::current_path() / "assets");
  content::sfml::registerSFMLLoaders(m_content);

  // setup graphics
  m_window.create(sf::VideoMode(800, 600), "Gravity");
  m_window.setVerticalSyncEnabled(true);

  m_states.push(makeGameState<NullState>(*this));
}

void Game::run() {
  sf::Clock clock;
  while (m_window.isOpen() && !m_states.empty()) {

    sf::Time elapsed = clock.restart();

    // as a side effect, this increases the ref-counter of the game state,
    // ensuring that it will not be destructed while it is still in use.
    GameStatePtr topState = m_states.top();

    topState->handleEvents();
    topState->update(elapsed);

    m_window.clear(sf::Color::Blue);
    m_window.draw(*topState);
    m_window.display();
  }
}
