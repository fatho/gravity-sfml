#pragma once

#include "sfml/textureloader.hpp"

namespace octo {
namespace content {

class ContentManager;

namespace sfml {

void registerSFMLLoaders(content::ContentManager& manager);

}
}
}
