#pragma once

#include "gamestate.hpp"

#include "content/contentmanager.hpp"

#include <memory>
#include <stack>

#include <SFML/Graphics.hpp>

namespace octo {

class GameState;

class Game final {
public:
  Game();

  void run();

  //////////////////////////////////////////
  // accessors

  sf::RenderWindow& window() {
    return m_window;
  }

  content::ContentManager& content() {
    return m_content;
  }

private:
  sf::RenderWindow m_window;
  std::stack<GameStatePtr> m_states;

  content::ContentManager m_content;
};
}
