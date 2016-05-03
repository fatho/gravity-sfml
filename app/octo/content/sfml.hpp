#pragma once

#include "contentmanager.hpp"

namespace octo {
namespace content {

class ContentManager;

namespace sfml {

template<typename ContentType>
class BasicSFMLLoader : public ContentLoader {
  // ContentLoader interface
public:
  std::shared_ptr<void> load(ContentManager& manager, const std::string& contentPath) override {
    auto stream = manager.openDataStream(contentPath);
    auto content = std::make_shared<ContentType>();
    if (stream && content->loadFromStream(*stream)) {
      return content;
    } else {
      throw ContentLoadException("failed to load content");
    }
  }
};

template<typename ContentType>
static void registerSFMLLoader(octo::content::ContentManager& manager) {
  manager.registerLoader<ContentType>(std::make_unique<BasicSFMLLoader<ContentType>>());
}

void registerSFMLLoaders(content::ContentManager& manager);

} // namespace sfml
} // namespace content
} // namespace octo
