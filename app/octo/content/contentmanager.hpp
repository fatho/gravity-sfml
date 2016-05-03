#pragma once

#include <exception>
#include <memory>
#include <string>
#include <typeinfo>

#include <SFML/System/InputStream.hpp>

namespace octo {
namespace content {

class ContentLoader {
public:
  virtual std::shared_ptr<void> load(sf::InputStream& stream) = 0;

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

  template <class AssetType>
  std::shared_ptr<AssetType> load(const std::string& assetId) {
    return load(typeid(AssetType), assetId);
  }

  template <class AssetType>
  void registerLoader(std::unique_ptr<ContentLoader> loader) {
    registerLoader(typeid(AssetType), std::move(loader));
  }

  void registerLoader(const std::type_info& assetType, std::unique_ptr<ContentLoader>);

  std::shared_ptr<void> load(const std::type_info& assetType, const std::string& assetId);

private:
  class Impl;
  Impl * const m_impl;
};
}
}
