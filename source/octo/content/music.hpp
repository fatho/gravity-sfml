#pragma once

#include "contentmanager.hpp"

#include <memory>

#include <SFML/Audio/Music.hpp>
#include <SFML/System/InputStream.hpp>

namespace octo {
namespace content {

/*! \brief A class wrapping a SFML music object together with the underlying input stream.
 *
 *  The reasoning behind this class is to avoid keeping separately track of the input stream
 *  associated with a sf::Music, as the latter becomes invalid as soon as the stream is
 *  destructed.
 */
class MusicContent {
public:
  /*! \brief Creates a music object from a stream.
   *
   *  \param musicStream the underlying music data stream
   */
  MusicContent(std::unique_ptr<sf::InputStream> musicStream);

  /// the underlying sf::Music object
  sf::Music& music();

private:
  /*! \brief The music data stream.
   *
   *  \note It is required to be kept alive as long as the music object exists.
   *  Therefore, it is important that this member is listed above MusicContent::m_music.
   */
  std::unique_ptr<sf::InputStream> m_musicStream;
  /*! \brief The underlying music, referencing MusicContent::m_musicStream.
   *  \note This member must be listed below the stream member, in order to ensure that
   *  the music object is destructed before the stream.
   */
  sf::Music m_music;
};

/*! \brief A content loader for SFML music objects.
 *
 *  This content loader automatically encapsulates the sf::Music together with the input stream,
 *  to prevent the stream from going out of scope while the music is still active.
 */
class MusicLoader : public ContentLoader {
public:
  std::shared_ptr<void> load(ContentManager& manager, const std::string& contentPath) override;
};

}
}
