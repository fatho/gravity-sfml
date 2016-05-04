#include "game.hpp"
#include "gamestate.hpp"
#include "content/sfml.hpp"

#include <fmtlog/log.hpp>

#include <boost/filesystem.hpp>

using namespace octo;

Game::Game() {
  // TODO: load settings

  // setup subsystems
  m_content.setBasePath(boost::filesystem::current_path() / ".." / "assets");
  content::sfml::registerSFMLLoaders(m_content);

  // setup graphics
  m_window.create(sf::VideoMode(800, 600), "Gravity");
  m_window.setVerticalSyncEnabled(true);

  pushNewState<NullState>();
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


void Game::pushState(GameStatePtr state) {
  assert(state);
  if(m_states.size() > 0) {
    // previous top state becomes obscured
    m_states.top()->deactivated();
  }
  m_states.push(state);
  // register new state
  state->setGame(this);
  state->added();
  state->activated();
}

void Game::popState() {
  assert(!m_states.empty());

  GameStatePtr state = topState();
  // unregister old state
  m_states.pop();
  state->deactivated();
  state->removed();
  state->setGame(nullptr);
  // new top state becomes active
  if(m_states.size() > 0) {
    m_states.top()->activated();
  }
}

void Game::changeState(GameStatePtr state) {
  assert(state);
  assert(!m_states.empty());
  // not implemented in terms of pushState/popState because semantics
  // for game state notifications are different here
  GameStatePtr removed = m_states.top();
  m_states.pop();
  removed->deactivated();
  removed->removed();
  removed->setGame(nullptr);
  state->setGame(this);
  m_states.push(state);
  state->added();
  state->activated();
}

GameStatePtr Game::topState() {
  return m_states.top();
}

