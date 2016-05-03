#include "contentmanager.hpp"

#include <string>
#include <typeindex>
#include <unordered_map>

#include <boost/format.hpp>
#include <SFML/System/FileInputStream.hpp>

using namespace octo::content;

ContentLoader::~ContentLoader() {}

ContentLoadException::ContentLoadException(const char* message) : std::runtime_error(message) {}

ContentLoadException::ContentLoadException(const std::string& message)
    : std::runtime_error(message) {}

class ContentManager::Impl {
public:
  std::unordered_map<std::type_index, std::unique_ptr<ContentLoader>> loaders;
  std::unordered_map<std::string, std::weak_ptr<void>> contentCache;
};

ContentManager::ContentManager() : m_impl(new Impl) {}

ContentManager::~ContentManager() {
  delete m_impl;
}

void ContentManager::registerLoader(const std::type_info& ContentType,
                                    std::unique_ptr<ContentLoader> loader) {
  if (loader) {
    m_impl->loaders.insert(std::make_pair(std::type_index(ContentType), std::move(loader)));
  } else {
    m_impl->loaders.erase(ContentType);
  }
}

std::shared_ptr<void> ContentManager::load(const std::type_info& contentType,
                                           const std::string& contentId, bool useCache) {
  using boost::format;
  if (useCache) {
    // check if content is still in cache and weak_ptr has not yet expired
    auto contentIt = m_impl->contentCache.find(contentId);
    if (contentIt != m_impl->contentCache.end()) {
      if (auto ContentPtr = (*contentIt).second.lock()) {
        return ContentPtr;
      }
    }
  }

  // otherwise, load (again)
  auto loaderIt = m_impl->loaders.find(contentType);
  if (loaderIt != m_impl->loaders.end()) {
    ContentLoader& loader = *(*loaderIt).second;
    auto contentPtr = loader.load(*this, contentId);
    if(useCache) {
      m_impl->contentCache.insert(std::make_pair(contentId, contentPtr));
    }
    return contentPtr;
  } else {
    throw ContentLoadException(
        boost::str(format("no Content loader for Content type '%s'") % contentType.name()));
  }
}
