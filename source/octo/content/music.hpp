#pragma once

#include "contentmanager.hpp"
#include "streaming.hpp"

#include <memory>
#include <type_traits>

#include <SFML/Audio/Music.hpp>
#include <SFML/System/InputStream.hpp>

namespace octo {
namespace content {

template<>
struct Streamable<sf::Music> {
  static bool open(sf::Music& music, sf::InputStream& stream) {
    return music.openFromStream(stream);
  }
};

using MusicContent = StreamingContent<sf::Music>;
using MusicLoader = StreamingLoader<sf::Music>;

}
}
