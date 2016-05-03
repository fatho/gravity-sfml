#include "sfml.hpp"
#include "contentmanager.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>

void octo::content::sfml::registerSFMLLoaders(octo::content::ContentManager& manager) {
  registerSFMLLoader<sf::Texture>(manager);
  registerSFMLLoader<sf::Image>(manager);
  registerSFMLLoader<sf::Font>(manager);
  registerSFMLLoader<sf::Sound>(manager);
  registerSFMLLoader<sf::Music>(manager);


  // TODO: add other SFML content handlers
}
