#include "filecontentmanager.hpp"

#include <boost/format.hpp>
#include <SFML/System/FileInputStream.hpp>

using namespace octo::content;
using namespace boost;

std::unique_ptr<sf::InputStream>
FileContentManager::openDataStream(const std::string& contentPath) {
  auto stream = std::make_unique<sf::FileInputStream>();
  if (stream->open((m_basePath / contentPath).string())) {
    return std::move(stream);
  } else {
    throw ContentLoadException(str(format("unable to open stream for content path '%s'") % contentPath));
  }
}

bool FileContentManager::exists(const std::string& contentPath) {
  return filesystem::exists(m_basePath / contentPath);
}


const filesystem::path& FileContentManager::basePath() const {
  return m_basePath;
}

void FileContentManager::setBasePath(const filesystem::path& basePath) {
  m_basePath = basePath;
}

FileContentManager::FileContentManager(const boost::filesystem::path& basePath)
    : m_basePath(basePath) {}

FileContentManager::FileContentManager() {}
