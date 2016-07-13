#include "contentmanager.hpp"

#include <SFML/System/FileInputStream.hpp>
#include <boost/format.hpp>

using namespace octo::content;

ContentLoader::~ContentLoader() {}

ContentLoadException::ContentLoadException(const char* message) : std::runtime_error(message) {}

ContentLoadException::ContentLoadException(const std::string& message)
    : std::runtime_error(message) {}

ContentManager::ContentManager() {}

ContentManager::~ContentManager() {}

void ContentManager::registerLoader(const boost::typeindex::type_index& ContentType,
                                    std::unique_ptr<ContentLoader> loader) {
  if (loader) {
    auto ptr = loader.get();
    log.debug("registered loader %s for content type %s",
              boost::typeindex::type_id_runtime(*ptr).pretty_name(),
              ContentType.pretty_name());
    this->m_loaders.insert(
        std::make_pair(boost::typeindex::type_index(ContentType), std::move(loader)));
  } else {
    this->m_loaders.erase(ContentType);
    log.debug("unregistered loader for %s", ContentType.name());
  }
}

std::shared_ptr<void> ContentManager::load(const boost::typeindex::type_index& contentType,
                                           const std::string& contentId, bool useCache) {
  using boost::format;
  if (useCache) {
    // check if content is still in cache and weak_ptr has not yet expired
    auto contentIt = this->m_contentCache.find(contentId);
    if (contentIt != this->m_contentCache.end()) {
      if (auto ContentPtr = (*contentIt).second.lock()) {
        log.debug("cache hit for %s", contentId);
        return ContentPtr;
      }
    }
  }

  // otherwise, load (again)
  auto loaderIt = this->m_loaders.find(contentType);
  if (loaderIt != this->m_loaders.end()) {
    ContentLoader& loader = *(*loaderIt).second;
    auto contentPtr = loader.load(*this, contentId);
    log.debug("loaded %s", contentId);
    if (useCache) {
      this->m_contentCache.insert(std::make_pair(contentId, contentPtr));
    }
    return contentPtr;
  } else {
    throw ContentLoadException(
        boost::str(format("no Content loader for Content type '%s'") % contentType.name()));
  }
}

void ContentManager::cleanupCache() {
  // remove all expired references
  for(auto iter = begin(m_contentCache); iter != end(m_contentCache);) {
    if(iter->second.expired()) {
      iter = m_contentCache.erase(iter);
    } else {
      ++iter;
    }
  }
}
