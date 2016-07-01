#pragma once

#include "contentmanager.hpp"

#include <string>
#include <cpp-physfs/physfs.hpp>

namespace octo {
namespace content {

/*! \brief A content manager loading files from the current PhysFS context.
 */
class PhysFSContentManager : public ContentManager {
public:
  std::unique_ptr<sf::InputStream> openDataStream(const std::string& contentPath) override;

  bool exists(const std::string& contentPath) override;
};

} // namespace content
} // namespace octo
