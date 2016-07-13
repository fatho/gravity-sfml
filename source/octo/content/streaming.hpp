#pragma once

#include "contentmanager.hpp"
#include <fmtlog/fmtlog.hpp>

#include <boost/format.hpp>
#include <SFML/System/InputStream.hpp>

#include <memory>

namespace octo {
namespace content {


/*! \brief Streamable concept/trait.
 *
 *  Must be specialized and provide \code static bool open(T&, sf::InputStream&) \endcode.
 */
template<class T>
struct Streamable;

/*! \brief A wrapper for (SFML) content that requires streaming.
 *
 *  This content object couples the life time of the underlying stream to the
 *  lifetime of the content object, making sure that the stream is only destroyed
 *  after the content object.
 *
 *  \tparam ContentType A default constructible content type.
 *  \tparam StreamLoadFn A pointer to a member function of ContentType,
 *  initializing the object from a stream.
 */
template <class ContentType>
class StreamingContent {
public:
  /*! \brief Creates a music object from a stream.
   *
   *  \param musicStream the underlying music data stream
   */
  StreamingContent(std::unique_ptr<sf::InputStream> contentStream)
    : m_contentStream(std::move(contentStream)) {
    if (!m_contentStream || !Streamable<ContentType>::open(m_content, *m_contentStream)) {
      throw ContentLoadException(boost::format("could not load streaming content").str());
    }
    log.debug("created: %p", m_contentStream.get());
  }

  /// returns the underlying content object
  ContentType& content() {
    return m_content;
  }

  ~StreamingContent() {
    log.debug("deleted: %p", m_contentStream.get());
  }

private:
  fmtlog::Log log = fmtlog::For<StreamingContent<ContentType>>();
  std::unique_ptr<sf::InputStream> m_contentStream;
  ContentType m_content;
};

template <class ContentType>
class StreamingLoader : public ContentLoader {
public:
  std::shared_ptr<void> load(ContentManager& manager, const std::string& contentPath) override {
    return std::make_shared<StreamingContent<ContentType>>(manager.openDataStream(contentPath));
  }
};

}
}
