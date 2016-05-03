#pragma once
#include "../contentmanager.hpp"

namespace octo {
namespace content {
namespace sfml {

class TextureLoader : public ContentLoader {
  // ContentLoader interface
public:
  std::shared_ptr<void> load(sf::InputStream& stream) override;
};

} // namespace sfml
} // namespace content
} // namespace octo

