#pragma once

#include "gamestate.hpp"

#include "content/filecontentmanager.hpp"

#include <memory>
#include <stack>

#include <SFML/Graphics.hpp>

namespace octo {

class GameState;

class Game final {
public:
  Game();

  void run();

  template<class StateType, typename... Args>
  void pushNewState(Args&&... args) {
    pushState(makeGameState<StateType>(std::forward<Args>(args)...));
  }
  void pushState(GameStatePtr state);
  void popState();
  template<class StateType, typename... Args>
  void changeNewState(Args&&... args) {
    changeState(makeGameState<StateType>(std::forward<Args>(args)...));
  }
  void changeState(GameStatePtr state);
  GameStatePtr topState();

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

  content::FileContentManager m_content;
};
}
