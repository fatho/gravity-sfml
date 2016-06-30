#pragma once

#include <exception>
#include <memory>
#include <string>
#include <typeinfo>

#include <SFML/System/InputStream.hpp>

namespace octo {
namespace content {

class ContentManager;

/*! \brief Abstract base class for content loaders.
 *  A content loader is responsible for loading content of a given type.
 */
class ContentLoader {
public:
  /*! \brief loads an asset
   *  \param manager the calling content manager
   *  \param contentPath the virtual path of the asset file
   */
  virtual std::shared_ptr<void> load(ContentManager& manager, const std::string& contentPath) = 0;

  /// required virtual destructor
  virtual ~ContentLoader();
};

/*! \brief Signals an error during content loading.
 */
class ContentLoadException : public std::runtime_error {
public:
  /*! \brief Initializes the exception object with a plain text message.
   *  \param message the error message
   *  \todo provide predefined error codes/types
   */
  ContentLoadException(const char* message);
  /*! \brief Initializes the exception object with a plain text message.
   *  \param message the error message
   */
  ContentLoadException(const std::string& message);
};

/*! \brief A content manager provides access to the underlying asset files and caching of loaded assets.
 *  Base classes need to define a way of turning a virtual content path into an actual stream.
 */
class ContentManager {
public:
  /// initializes an empty content manager
  ContentManager();
  /// required virtual destructor
  virtual ~ContentManager();

public:
  /*! \brief opens a data stream from a content file
   *  \param contentPath the virtual path of the content file
   *  \returns a uniquely owned SFML input stream
   *  \exception ContentLoadException for any error that occurs during the stream creation
   */
  virtual std::unique_ptr<sf::InputStream> openDataStream(const std::string& contentPath) = 0;

  /*! \brief checks whether content exists under the given path
   *  \param contentPath the content path to check
   *  \returns a boolean indicating whether the content file exists
   */
  virtual bool exists(const std::string& contentPath) = 0;

  /*! \brief loads a content file of a given asset type
   *  This function template automatically chooses the right content loader for the given \p AssetType
   *  from the registered loaders.
   *  \param contentPath the virtual path of the content
   *  \param useCache If true, this function is allowed to return a shared pointer to the element in the cache.
   *  If false, it will always load a fresh copy.
   */
  template <class AssetType>
  std::shared_ptr<AssetType> load(const std::string& contentPath, bool useCache = true) {
    return load(typeid(AssetType), contentPath, useCache);
  }

  /*! \brief registers a ContentLoader with this content manager
   *  This function template takes ownership of a content loader for assets of type \p AssetType
   *  and registers it for use in ContentManager::load.
   *  \param loader The new loader used for assets of type \p AssetType. May be null to unregister.
   */
  template <class AssetType>
  void registerLoader(std::unique_ptr<ContentLoader> loader) {
    registerLoader(typeid(AssetType), std::move(loader));
  }

  /*! \brief registers a ContentLoader with this content manager
   *  This function takes ownership of a content loader for assets of type \p assetType
   *  and registers it for use in ContentManager::load.
   *  \param assetType the runtime representation of the asset type
   *  \param loader The new loader used for assets of type \p assetType. May be null to unregister.
   */
  void registerLoader(const std::type_info& assetType, std::unique_ptr<ContentLoader> loader);

  /*! \brief loads a content file of a given asset type
   *  This function template automatically chooses the right content loader for the given \p assetType
   *  from the registered loaders.
   *  \param assetType the runtime representation of the asset type
   *  \param contentPath the virtual path of the content
   *  \param useCache If true, this function is allowed to return a shared pointer to the element in the cache.
   *  If false, it will always load a fresh copy.
   */
  std::shared_ptr<void> load(const std::type_info& assetType, const std::string& contentPath, bool useCache);

private:
  class Impl;
  /// private implementation details
  Impl * const m_impl;
};
}
}
