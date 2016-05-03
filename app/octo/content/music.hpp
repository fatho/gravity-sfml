#pragma once

#include "contentmanager.hpp"

#include <memory>

#include <SFML/Audio/Music.hpp>
#include <SFML/System/InputStream.hpp>

namespace octo {
namespace content {

class MusicContent {
public:
  MusicContent(std::unique_ptr<sf::InputStream> musicStream);

  sf::Music& music();

private:
  sf::Music m_music;
  std::unique_ptr<sf::InputStream> m_musicStream;
};


class MusicLoader : public ContentLoader {
public:
  std::shared_ptr<void> load(ContentManager& manager, const std::string& contentPath) override;
};

}
}
