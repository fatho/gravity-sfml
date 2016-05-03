#pragma once

#include "contentmanager.hpp"

#include <memory>

namespace octo {
namespace content {

class ShaderLoader : public ContentLoader {
public:
  std::shared_ptr<void> load(ContentManager& manager, const std::string& contentPath) override;
};

}
}
