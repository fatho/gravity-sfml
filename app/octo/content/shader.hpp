#pragma once

#include "contentmanager.hpp"

#include <memory>

namespace octo {
namespace content {

/*! \brief A content loader for shaders.
 */
class ShaderLoader : public ContentLoader {
public:

  /*! \brief Loads a shader asset.
   *
   *  It looks for the vertex shader in \p contentPath with the additional extension <tt>.vert</tt>,
   *  and for the fragment shader in \p contentPath with appended extension <tt>.frag</tt>.
   *  When this function is invoked on the path <tt>shaders/foo</tt>, it will look for
   *    - <tt>shaders/foo.vert</tt> to load the vertex shader, and
   *    - <tt>shaders/foo.frag</tt> to load the fragment shader.
   *  A shader can be loaded if one or both files are present.
   *
   *  \param manager the calling content manager
   *  \param contentPath the base name of the shader
   *  \returns a shared pointer to the loaded shader object
   *  \exception ContentLoadException if neither of the shader files are present,
   *  or loading the shader files failed for another reason.
   */
  std::shared_ptr<void> load(ContentManager& manager, const std::string& contentPath) override;
};

}
}
