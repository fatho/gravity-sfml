#pragma once

#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>

namespace octo {

class Game;

/*! \brief Base class defining the interface of game states.
 *
 *  Game states are intrusively reference counted, i.e. the reference count is stored
 *  as part of this base class, and is accessed through \ref GameStatePtr.
 */
class GameState : public boost::intrusive_ref_counter<GameState> {
public:
  /*! \brief Returns a pointer to the game this state is currently registered with.
   *  \returns a pointer to a Game, or \c nullptr if this state is currently not registered with a game.
   */
  Game* game();

  /*! \brief Handles events in a derived class.
   *
   *  It is called by the Game on the top game state as part of the game loop.
   *
   *  \note This function is required to process all events in the SFML event queue,
   *  and react appropriately.
   */
  virtual void handleEvents() = 0;
  /*! \brief Handles game updates.
   *
   *  It is called by the Game on the top game state as part of the game loop.
   *
   *  \param elapsed the amount of time that elapsed since the last frame
   */
  virtual void update(sf::Time elapsed) = 0;

  /*! \brief Renders the game state to a specific render target.
   *
   *  \param target the render target.
   */
  virtual void draw(sf::RenderTarget& target) = 0;

  /// Virtual destructor.
  virtual ~GameState();


protected:
  friend class Game;

  /// Called by Game when registering a state.
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
  /// the game this state is registered on
  Game* m_game;
};

/*! \brief A reference counting pointer to a GameState.
 *
 *  This is implemented in terms of boosts \c intrusive_ptr,
 *  but that should be considered an implementation detail.
 */
typedef boost::intrusive_ptr<GameState> GameStatePtr;

/*! \brief Creates a new instance of \c GameStateClass, passing \p args to the constructor,
 *  and wraps the result in a managed pointer.
 *
 *  \param args the constructor arguments passed to \c GameStateClass
 */
template <class GameStateClass, typename... Args>
GameStatePtr makeGameState(Args&&... args) {
  return GameStatePtr(new GameStateClass(std::forward<Args>(args)...));
}

/*! \brief A dummy state waiting for the window to be closed.
 */
class NullState : public GameState {
public:
  void update(sf::Time elapsed) override;
  void handleEvents() override;

protected:
  void draw(sf::RenderTarget& target) override;
};
}
