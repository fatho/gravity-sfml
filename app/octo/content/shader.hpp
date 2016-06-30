#pragma once

#include "contentmanager.hpp"

#include <memory>

namespace octo {
namespace content {

/*! \brief A content loader for shaders, automatically detecting associated vertex/geometry programs.
 */
class ShaderLoader : public ContentLoader {
public:
  /*! \brief loads a shader asset
   *  It looks for the vertex shader in \p contentPath with the additional extension ".vert",
   *  and for the fragment shader in \p contentPath with appended extension ".frag".
   *  \param manager the calling content manager
   *  \param contentPath the base name of the shader
   */
  std::shared_ptr<void> load(ContentManager& manager, const std::string& contentPath) override;
};

}
}
