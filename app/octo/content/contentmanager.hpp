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
 *
 *  A content loader is responsible for loading content of a given type.
 */
class ContentLoader {
public:
  /*! \brief Loads an asset object.
   *
   *  Repeated invocations with the same content manager and path should
   *  return a fresh copy of the content object every time.
   *  \param manager content manager used for resolving paths
   *  \param contentPath virtual (base) path of the asset file.
   *  Might be extended for assets that consist of more than one physical content file.
   *  \returns a non-null shared pointer to a valid content object.
   *  \exception ContentLoadException if loading the content failed for any reason.
   *  \note Since the actual type of the returned object depends on the derieved classes,
   *  the return value is a \p void pointer. In order to nonetheless preserve the deleter,
   *  a \c shared_ptr is used instead of a \c unique_ptr.
   */
  virtual std::shared_ptr<void> load(ContentManager& manager, const std::string& contentPath) = 0;

  /// Virtual destructor.
  virtual ~ContentLoader();
};

/*! \brief Signals an error during content loading.
 *
 *  This usually indicates that content is missing or malformed.
 */
class ContentLoadException : public std::runtime_error {
public:
  /*! \brief Initializes the exception object with a plain text message.
   *
   *  \param message a plain text error message
   *  \todo provide predefined error codes/types
   */
  explicit ContentLoadException(const char* message);
  /*! \brief Initializes the exception object with a plain text message.
   *
   *  \param message a plain text error message
   */
  explicit ContentLoadException(const std::string& message);
};

/*! \brief A content manager provides access to the underlying asset files and caching of loaded assets.
 *
 *  Base classes are required to overload ContentManager::openDataStream in order to define
 *  a way of turning a virtual content path into an actual stream.
 */
class ContentManager {
public:
  /// Create a content manager without any loaders.
  ContentManager();
  /// Virtual destructor.
  virtual ~ContentManager();

public:
  /*! \brief Opens a data stream from a content file.
   *
   *  \param contentPath the virtual path of the content file
   *  \returns a non-null pointer to a valid SFML input stream
   *  \exception ContentLoadException if an error occurred during stream creation.
   */
  virtual std::unique_ptr<sf::InputStream> openDataStream(const std::string& contentPath) = 0;

  /*! \brief Checks whether content exists under the given path.
   *
   *  \param contentPath the content path to check
   *  \returns a boolean indicating whether the content file exists
   */
  virtual bool exists(const std::string& contentPath) = 0;

  /*! \brief Loads a content file of a given asset type.
   *
   *  This function template automatically chooses the right content loader for the given \p AssetType
   *  from the registered loaders.
   *  \param contentPath the virtual path of the content
   *  \param useCache indicates whether this function is allowed to return
   *  a shared pointer to the element in the cache. If false, it will always return a fresh copy.
   *  \returns a non-null pointer to the loaded content object.
   *  \exception ContentLoadException if loading the content failed for any reason.
   */
  template <class AssetType>
  std::shared_ptr<AssetType> load(const std::string& contentPath, bool useCache = true) {
    return load(typeid(AssetType), contentPath, useCache);
  }

  /*! \brief Registers a ContentLoader with this content manager.
   *
   *  This function template takes ownership of a content loader for assets of type \p AssetType
   *  and registers it for use in ContentManager::load.
   *  \param loader the new loader used for assets of type \p AssetType. May be null to unregister.
   */
  template <class AssetType>
  void registerLoader(std::unique_ptr<ContentLoader> loader) {
    registerLoader(typeid(AssetType), std::move(loader));
  }

  /*! \brief Registers a ContentLoader with this content manager.
   *
   *  This function takes ownership of a content loader for assets of type \p assetType
   *  and registers it for use in ContentManager::load.
   *  \param assetType the runtime representation of the asset type.
   *  \param loader the new loader used for assets of type \p assetType. May be null to unregister.
   */
  void registerLoader(const std::type_info& assetType, std::unique_ptr<ContentLoader> loader);

  /*! \brief Loads a content file of a given asset type.
   *
   *  This function template automatically chooses the right content loader for the given \p assetType
   *  from the registered loaders.
   *  \param assetType the runtime representation of the asset type
   *  \param contentPath the virtual path of the content
   *  \param useCache indicates whether this function is allowed to return
   *  a shared pointer to the element in the cache. If false, it will always return a fresh copy.
   *  \returns a non-null pointer to the loaded content object, can be cast to a shared pointer
   *  of the type represented by \p assetType.
   *  \exception ContentLoadException if loading the content failed for any reason.
   */
  std::shared_ptr<void> load(const std::type_info& assetType, const std::string& contentPath, bool useCache);

private:
  class Impl;
  /// private implementation details
  Impl * const m_impl;
};
}
}
