#pragma once

#include <exception>
#include <memory>
#include <string>
#include <typeinfo>

#include <SFML/System/InputStream.hpp>

namespace octo {
namespace content {

class ContentManager;

class ContentLoader {
public:
  virtual std::shared_ptr<void> load(ContentManager& manager, const std::string& contentPath) = 0;

  virtual ~ContentLoader();
};

class ContentLoadException : public std::runtime_error {
public:
  ContentLoadException(const char* message);
  ContentLoadException(const std::string& message);
};

class ContentManager {
public:
  ContentManager();
  ~ContentManager();

public:
  virtual std::unique_ptr<sf::InputStream> openDataStream(const std::string& contentPath) = 0;

  template <class AssetType>
  std::shared_ptr<AssetType> load(const std::string& assetId, bool useCache = true) {
    return load(typeid(AssetType), assetId, useCache);
  }

  template <class AssetType>
  void registerLoader(std::unique_ptr<ContentLoader> loader) {
    registerLoader(typeid(AssetType), std::move(loader));
  }

  void registerLoader(const std::type_info& assetType, std::unique_ptr<ContentLoader>);

  std::shared_ptr<void> load(const std::type_info& assetType, const std::string& assetId, bool useCache);

private:
  class Impl;
  Impl * const m_impl;
};
}
}
