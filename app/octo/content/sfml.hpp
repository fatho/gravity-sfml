#pragma once

#include "contentmanager.hpp"

namespace octo {
namespace content {

class ContentManager;

namespace sfml {

template<typename ContentType>
class SFMLLoader : public ContentLoader {
  // ContentLoader interface
public:
  std::shared_ptr<void> load(sf::InputStream& stream) override {
    auto content = std::make_shared<ContentType>();
    if (!content->loadFromStream(stream)) {
      throw ContentLoadException("failed to load content");
    }
    return content;
  }
};

template<typename ContentType>
static void registerSFMLLoader(octo::content::ContentManager& manager) {
  manager.registerLoader<ContentType>(std::make_unique<SFMLLoader<ContentType>>());
}

void registerSFMLLoaders(content::ContentManager& manager);

} // namespace sfml
} // namespace content
} // namespace octo
