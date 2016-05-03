#pragma once

#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>

namespace octo {

class Game;

class GameState : public sf::Drawable, public boost::intrusive_ref_counter<GameState> {
public:
  GameState(Game& game);

  Game& game();

  virtual void handleEvents() = 0;
  virtual void update(sf::Time elapsed) = 0;

  virtual ~GameState();

private:
  Game& m_game;
};

using GameStatePtr = boost::intrusive_ptr<GameState>;

template <class GameStateClass, typename... Args>
GameStatePtr makeGameState(Args&&... args) {
  return GameStatePtr(new GameStateClass(std::forward<Args>(args)...));
}

class NullState : public GameState {
public:
  NullState(Game& game);

  void update(sf::Time elapsed) override;
  void handleEvents() override;

protected:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
}
