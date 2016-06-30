#pragma once

#include "contentmanager.hpp"

#include <string>
#include <boost/filesystem.hpp>

namespace octo {
namespace content {

/*! \brief A file system based content manager.
 *   This content manager resolves content paths relative to some base directory.
 */
class FileContentManager : public ContentManager {
public:
  /// a content manager using the current directory as base path
  FileContentManager();
  /*! \brief initializes the content manager with a base path
   *  \param basePath the path to the base directory used for resolving content paths
   */
  FileContentManager(const boost::filesystem::path& basePath);

  /*! \brief sets a new base directory
   *  \param basePath the path of the base directory
   */
  void setBasePath(const boost::filesystem::path& basePath);

  /*! \brief returns the current base path
   *  \return the path of the current base directory
   */
  const boost::filesystem::path& basePath() const;

  /*! \brief opens a data stream to the file specified relative to FileContentManager::basePath.
   *  \param contentPath the virtual content path relative to the base directory
   *  \exception ContentLoadException signals that the stream could not be opened
   */
  std::unique_ptr<sf::InputStream> openDataStream(const std::string& contentPath) override;

  /*! \brief checks whether the file relative to the base directory exists
   *  \param contentPath the content path to check
   *  \returns a boolean indicating whether the content file exists
   */
  bool exists(const std::string& contentPath) override;

private:
  /// the path of the base directory
  boost::filesystem::path m_basePath;
};

} // namespace content
} // namespace octo
