#pragma once

#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>

namespace octo {

class Game;

class GameState : public sf::Drawable, public boost::intrusive_ref_counter<GameState> {
public:
  Game* game();

  virtual void handleEvents() = 0;
  virtual void update(sf::Time elapsed) = 0;

  virtual ~GameState();


protected:
  friend class Game;

  void setGame(Game* game);

  /// called when added to the game
  virtual void added();
  /// called when removed from the game
  virtual void removed();
  /// called when the state becomes the new active (= top) state
  virtual void activated();
  /// called when the state becomes deactivated (i.e. it is no longer the top state)
  virtual void deactivated();

private:
  Game* m_game;
};

using GameStatePtr = boost::intrusive_ptr<GameState>;

template <class GameStateClass, typename... Args>
GameStatePtr makeGameState(Args&&... args) {
  return GameStatePtr(new GameStateClass(std::forward<Args>(args)...));
}

class NullState : public GameState {
public:
  void update(sf::Time elapsed) override;
  void handleEvents() override;

protected:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
}
