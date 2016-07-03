#include "game.hpp"
#include "gamestate.hpp"
#include "content/sfml.hpp"

#include "states/ingamestate.hpp"

#include <fmtlog/fmtlog.hpp>
#include <boost/filesystem.hpp>

using namespace octo;

Game::Game() {
  // TODO: load settings

  // setup subsystems
  log.info("initializing content manager");
  m_content.setBasePath(boost::filesystem::current_path() / "assets");
  content::sfml::registerSFMLLoaders(m_content);

  // setup graphics
  log.info("creating window");
  m_window.create(sf::VideoMode(800, 600), "Gravity");
  m_window.setVerticalSyncEnabled(true);

  // setup state
  log.info("creating initial state");
  pushNewState<states::InGameState>();

  log.info("initialization complete");
}

void Game::run() {
  log.info("starting game loop");
  sf::Clock clock;
  // FIXME: this loop relies on being capped by VSync
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
  log.info("game loop finished");
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
  state->deactivated();
  // unregister old state
  m_states.pop();
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
  removed->deactivated();
  m_states.pop();
  removed->removed();
  removed->setGame(nullptr);
  state->setGame(this);
  m_states.push(state);
  state->added();
  state->activated();
}

GameStatePtr Game::topState() {
  if(m_states.empty()) {
    return nullptr;
  } else {
    return m_states.top();
  }
}
