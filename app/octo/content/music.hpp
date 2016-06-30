#pragma once

#include "contentmanager.hpp"

#include <memory>

#include <SFML/Audio/Music.hpp>
#include <SFML/System/InputStream.hpp>

namespace octo {
namespace content {

/*! \brief A class wrapping a SFML music object together with the underlying input stream.
 *  The reasoning behind this class is to avoid keeping separately track of the input stream
 *  associated with a sf::Music, as the latter becomes invalid as soon as the stream is
 *  destructed.
 */
class MusicContent {
public:
  /*! \brief creates a music object from a stream
   *  \param musicStream the underlying music data stream
   */
  MusicContent(std::unique_ptr<sf::InputStream> musicStream);

  /// the underlying sf::Music object
  sf::Music& music();

private:
  /// the underlying music, referencing MusicContent::m_musicStream
  sf::Music m_music;
  /// the music data stream, required to be kept open as long as the music object exists
  std::unique_ptr<sf::InputStream> m_musicStream;
};

/*! \brief A content loader for SFML music objects.
 *  This content loader automatically encapsulates the sf::Music together with the input stream,
 *  to prevent the stream from going out of scope while the music is still active.
 */
class MusicLoader : public ContentLoader {
public:
  /*! \brief Loads a music object from a content file.
   *  \param manager the calling content manager
   *  \param contentPath the virtual path of the music file
   */
  std::shared_ptr<void> load(ContentManager& manager, const std::string& contentPath) override;
};

}
}
