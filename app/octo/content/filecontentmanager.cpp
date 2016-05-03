#include "filecontentmanager.hpp"

#include <boost/format.hpp>
#include <SFML/System/FileInputStream.hpp>

using namespace octo::content;
using namespace boost;
using namespace boost::filesystem;

std::unique_ptr<sf::InputStream>
FileContentManager::openDataStream(const std::string& contentPath) {
  auto stream = std::make_unique<sf::FileInputStream>();
  if (stream->open((m_basePath / contentPath).string())) {
    return std::move(stream);
  } else {
    return nullptr;
  }
}

const path& octo::content::FileContentManager::basePath() {
  return m_basePath;
}

void octo::content::FileContentManager::setBasePath(const path& basePath) {
  m_basePath = basePath;
}

octo::content::FileContentManager::FileContentManager(const path& basePath)
    : m_basePath(basePath) {}

octo::content::FileContentManager::FileContentManager() {}
