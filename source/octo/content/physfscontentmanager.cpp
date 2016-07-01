#include "physfscontentmanager.hpp"

#include <cpp-physfs/physfs.hpp>

using namespace octo::content;

std::unique_ptr<sf::InputStream>
PhysFSContentManager::openDataStream(const std::string& contentPath) try {
  return std::make_unique<physfs::InputStream>(contentPath);
} catch (physfs::IOException& ex) {
  throw ContentLoadException(ex.what());
}

bool PhysFSContentManager::exists(const std::string& contentPath) {
  return physfs::exists(contentPath);
}
