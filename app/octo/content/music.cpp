#include "music.hpp"

using namespace octo::content;

sf::Music& MusicContent::music() {
  return m_music;
}

MusicContent::MusicContent(std::unique_ptr<sf::InputStream> musicStream)
  : m_musicStream(std::move(musicStream)) {
  if(!m_musicStream || !m_music.openFromStream(*m_musicStream)) {
    throw ContentLoadException("could not load music");
  }
}

std::shared_ptr<void> MusicLoader::load( octo::content::ContentManager &manager, const std::string &contentPath) {
  return std::make_shared<MusicContent>(manager.openDataStream(contentPath));
}
