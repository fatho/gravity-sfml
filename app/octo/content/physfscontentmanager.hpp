#pragma once

#include "contentmanager.hpp"

#include <string>
#include <cpp-physfs/physfs.hpp>

namespace octo {
namespace content {

class PhysFSContentManager : public ContentManager {
public:

  std::unique_ptr<sf::InputStream> openDataStream(const std::string& contentPath);

};

} // namespace content
} // namespace octo
