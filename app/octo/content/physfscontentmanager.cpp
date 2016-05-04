#include "physfscontentmanager.hpp"

#include <cpp-physfs/physfs.hpp>
#include <boost/format.hpp>

using namespace octo::content;
using namespace boost;
using namespace boost::filesystem;

std::unique_ptr<sf::InputStream> PhysFSContentManager::openDataStream(const std::string& contentPath) {
  return std::make_unique<physfs::InputStream>(contentPath);
}
