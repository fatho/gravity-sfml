#pragma once

#include "contentmanager.hpp"
#include "streaming.hpp"

#include <memory>
#include <type_traits>

#include <SFML/Graphics/Font.hpp>
#include <SFML/System/InputStream.hpp>

namespace octo {
namespace content {

template<>
struct Streamable<sf::Font> {
  static bool open(sf::Font& font, sf::InputStream& stream) {
    return font.loadFromStream(stream);
  }
};

using FontContent = StreamingContent<sf::Font>;
using FontLoader = StreamingLoader<sf::Font>;

}
}
