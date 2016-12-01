#pragma once

#include <string>

namespace octo {
namespace game {
namespace components {

/*! \brief Component defining a planet.
 *
 *  The planets textures are referred to by their ids.
 *
 *  \todo add atmospheric parameters (that's for later)
 */
struct Planet {
  /*! \brief Texture ID of the planets background.
   *
   *  It fills all the parts of the planet that originally belonged to the planet but have holes now.
   */
  std::string backgroundTextureId;

  /*! \brief Texture ID of the planets foreground.
   */
  std::string foregroundTextureId;
};

}
}
}
