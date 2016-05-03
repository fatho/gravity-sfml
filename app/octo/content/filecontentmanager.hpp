#pragma once

#include "contentmanager.hpp"

#include <string>
#include <boost/filesystem.hpp>

namespace octo {
namespace content {

class FileContentManager : public ContentManager {
public:
  FileContentManager();
  FileContentManager(const boost::filesystem::path& basePath);

  void setBasePath(const boost::filesystem::path& basePath);
  const boost::filesystem::path& basePath();

  std::unique_ptr<sf::InputStream> openDataStream(const std::string& contentPath);

private:
  boost::filesystem::path m_basePath;
};

} // namespace content
} // namespace octo
