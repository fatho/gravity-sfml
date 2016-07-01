#pragma once

#include "contentmanager.hpp"

#include <string>
#include <boost/filesystem.hpp>

namespace octo {
namespace content {

/*! \brief A file system based content manager.
 *
 *   This content manager interprets virtual content paths relative to some base directory
 *   physically present in the file system.
 */
class FileContentManager : public ContentManager {
public:
  /*! \brief Initializes the content manager using the current directory as base path.
   */
  FileContentManager();
  /*! \brief Initializes the content manager with a base path.
   *
   *  \note The current directory is represented by the empty string.
   *  \param basePath the path to the base directory used for resolving content paths
   */
  FileContentManager(const boost::filesystem::path& basePath);

  /*! \brief Sets a new base directory.
   *
   *  \note The current directory is represented by the empty string.
   *  \param basePath the path of the base directory
   */
  void setBasePath(const boost::filesystem::path& basePath);

  /*! \brief The current base directory used for resolving content.
   *
   *  \returns the path of the current base directory
   */
  const boost::filesystem::path& basePath() const;

  std::unique_ptr<sf::InputStream> openDataStream(const std::string& contentPath) override;

  bool exists(const std::string& contentPath) override;

private:
  /// the path of the base directory
  boost::filesystem::path m_basePath;
};

} // namespace content
} // namespace octo
