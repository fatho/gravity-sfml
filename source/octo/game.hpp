#pragma once

#include "gamestate.hpp"

#include "content/filecontentmanager.hpp"

#include <memory>
#include <stack>

#include <SFML/Graphics.hpp>

namespace octo {

class GameState;

/*! \brief This is the root class containing the basic game logic.
 *
 *  It is mainly responsible for the game loop and game state management.
 *  The specific logic for individual parts of the game is contained in the game states.
 */
class Game final {
public:
  /*! \brief Initializes the game.
   *
   *  The following initialization steps are performed:
   *    1. game settings are read,
   *    2. game Window is created,
   *    3. any other important subsystems are loaded
   *    4. initial game state is created.
   *
   *  In particular, the game state being created in step 4 can expect an otherwise fully
   *  initialized Game.
   *
   *  \note This constructor only fails (with an exception) in case of critical failures,
   *  i.e. being unable to create the window, or other unexpected conditions.
   */
  Game();

  /*! \brief Runs the game loop.
   *
   *  \note This function blocks until the game is exited.
   */
  void run();

  //////////////////////////////////////////
  // State management

  /*! \brief Creates a new instance of a game state and pushes it on top of the state stack.
   *
   *  This is a convenience function that is equivalent to manually constructing the new
   *  instance and passing it to Game::pushState.
   *
   *  \param args the constructor arguments for \p StateType.
   *  \see Game::pushState
   */
  template <class StateType, typename... Args>
  void pushNewState(Args&&... args) {
    pushState(makeGameState<StateType>(std::forward<Args>(args)...));
  }

  /*! \brief Pushes a state on top of the game state stack.
   *
   *  Semantically, it performs the following operations in that order:
   *    1. notifies the previous state about being \link GameState::deactivated deactivated \endlink
   *    2. pushes new state to the stack
   *    3. notifies new state about being \link GameState::added added \endlink
   *    4. notifies new state about being \link GameState::activated activated \endlink
   *
   *  \param state a non-null pointer to a game state that is not registered with another Game.
   *  \see GameState for a description of the semantics of the individual notifications.
   */
  void pushState(GameStatePtr state);

  /*! \brief Removes the state on top of the game state stack.
   *
   *  Semantically, it performs the following operations in that order:
   *    1. notifies the top state about being \link GameState::deactivated deactivated \endlink
   *    2. removes the top state from the stack
   *    3. notifies removed state about being \link GameState::removed removed \endlink
   *    4. notifies new top state (if existent) about being \link GameState::activated activated \endlink
   *
   *  \see GameState for a description of the semantics of the individual notifications.
   */
  void popState();

  /*! \brief Creates a new instance of a game state and replaces the top state of the state stack with it.
   *
   *  This is a convenience function that is equivalent to manually constructing the new
   *  instance and passing it to Game::changeState.
   *  \see Game::changeState
   */
  template <class StateType, typename... Args>
  void changeNewState(Args&&... args) {
    changeState(makeGameState<StateType>(std::forward<Args>(args)...));
  }

  /*! \brief Creates a new instance of a game state and replaces the top state of the state stack with it.
   *
   *  Semantically, it performs the following operations in that order:
   *    1. notifies the top state about being \link GameState::deactivated deactivated \endlink
   *    2. removes the top state from the stack
   *    3. notifies removed state about being \link GameState::removed removed \endlink
   *    4. pushes new state to the stack
   *    5. notifies new state about being \link GameState::added added \endlink
   *    6. notifies new state about being \link GameState::activated activated \endlink
   *
   *  In contrast to calling first Game::popState and then Game::pushState, this function does not
   *  call any notification functions of the state below the top state.
   *
   *  \param state a non-null pointer to a game state that is not registered with another Game.
   *  \see GameState for a description of the semantics of the individual notifications.
   */
  void changeState(GameStatePtr state);

  /*! \brief Returns a pointer to the state currently on top of the state stack.
   *  \returns a pointer to the top state or \c null, if the state stack is empty.
   */
  GameStatePtr topState();

  //////////////////////////////////////////
  // Utility Functions

  //////////////////////////////////////////
  // Accessors

  /*! \brief Returns a reference to the render window.
   *
   *  The returned window is guaranteed to be valid as long as Game::run has not returned.
   *  In particular, before Game::run was called for the first time.
   *  \returns a reference to the underlying SFML window
   */
  sf::RenderWindow& window() {
    return m_window;
  }

  /*! \brief Returns a reference to the content manager used by the game.
   *
   *  The returned content manager is valid as long as the Game object is not destructed.
   *  \returns a reference to the content manager
   */
  content::ContentManager& content() {
    return m_content;
  }

private:
  /// the SFML window used for rendering the game
  sf::RenderWindow m_window;
  /// the game state stack
  std::stack<GameStatePtr> m_states;
  /// the content manager
  content::FileContentManager m_content;
};
}
