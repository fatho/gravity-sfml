#include "shader.hpp"

#include <boost/format.hpp>
#include <SFML/Graphics/Shader.hpp>

using namespace octo::content;
using namespace boost;

std::shared_ptr<void> ShaderLoader::load(ContentManager& manager, const std::string& contentPath) {
  auto shader = std::make_shared<sf::Shader>();

  auto vertexStream = manager.openDataStream(contentPath + ".vert");
  auto fragmentStream = manager.openDataStream(contentPath + ".frag");

  bool vertLoaded = false;
  bool fragLoaded = false;

  if(vertexStream) {
    if(shader->loadFromStream(*vertexStream, sf::Shader::Vertex)) {
      vertLoaded = true;
    } else {
      throw ContentLoadException(str(format("failed to load vertex shader for '%s'") % contentPath));
    }
  }

  if(fragmentStream) {
    if(shader->loadFromStream(*fragmentStream, sf::Shader::Fragment)) {
      fragLoaded = true;
    } else {
      throw ContentLoadException(str(format("failed to load fragment shader for '%s'") % contentPath));
    }
  }

  if(!vertLoaded && !fragLoaded) {
    throw ContentLoadException(str(format("neither vertex nor fragment shader found for '%s'") % contentPath));
  }

  return shader;
}
