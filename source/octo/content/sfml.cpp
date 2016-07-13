#include "sfml.hpp"
#include "contentmanager.hpp"
#include "music.hpp"
#include "shader.hpp"
#include "font.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>

using namespace octo::content::sfml;

void octo::content::sfml::registerSFMLLoaders(octo::content::ContentManager& manager) {
  // "easy" SFML assets (completely loaded in memory, consisting of one file)
  registerBasicSFMLLoader<sf::Texture>(manager);
  registerBasicSFMLLoader<sf::Image>(manager);
  registerBasicSFMLLoader<sf::Font>(manager);
  registerBasicSFMLLoader<sf::SoundBuffer>(manager);

  // music and font content requires some extra work to keep the underlying stream alive
  manager.registerLoader<MusicContent>(std::make_unique<MusicLoader>());
  manager.registerLoader<FontContent>(std::make_unique<FontLoader>());

  // shaders usually consist of two content files
  manager.registerLoader<sf::Shader>(std::make_unique<ShaderLoader>());
  
}
