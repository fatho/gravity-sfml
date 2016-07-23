/*! \file
 *  \brief Defines a modern C++ interface to PhysFS.
 *
 *  In addition to providing safe RAII wrappers for file handles and mount points,
 *  this file also provides a bridge from PhysFS files to SFML and Boost streams.
 */
#pragma once

#include <physfs.h>
#include <memory>
#include <string>
#include <streambuf>

#include <SFML/System/InputStream.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/iostreams/categories.hpp>

// hack to silence annoying deprecation warnings in the innards of boost
#undef __GNUC__

#include <boost/iostreams/stream.hpp>

// undo hack
#define __GNUC__

/*! \brief Namespace containing a modern C++ interface to PhysFS.
 */
namespace physfs {

/*!
 * \brief A general exception type wrapping all errors originating from the PhysFS library.
 */
struct IOException : std::ios_base::failure {
  /*! \brief Initializes the exception object with the PhysFS error that occurred.
   */
  IOException();
  /*! \brief Initializes the exception object with a user defined message.
   *  \param what_arg the error message.
   */
  explicit IOException(const char* what_arg);
  /*! \brief Initializes the exception object with a user defined message.
   *  \param what_arg the error message.
   */
  explicit IOException(const std::string& what_arg);
};

/*!
 * \brief Initializes PhysFS in the constructor and deinitializes in the
 * destructor.
 */
class Initializer {
public:
  /// Initializes PhysFS
  explicit Initializer(const char* argv0);

  /// Deinitializes PhysFS
  ~Initializer();
};

/*!
 * \brief Archives can be mounted in the beginning or the end of the
 */
enum SearchPathInsertion {
  Append, ///< The archive is added to the end of the search path.
  Prepend ///< The archive is added to the front of the search path.
};

/*!
 * \brief An RAII mount of a PhysFS archive.
 */
class Mount {
public:
  /*! Initializes the mount handle without mounting.
   */
  Mount();

  Mount(const Mount&) = delete;
  Mount& operator=(const Mount&) = delete;

  /*! \brief Moves the mounting scope from the argument to a new mount handle.
   */
  Mount(Mount&& other);
  /*! \brief Moves the mounting scope from the argument to a this mount handle.
   */
  Mount& operator=(Mount&&);

  /*!
   * \brief Mounts an archive to a virtual path.
   * \param archive_path the physical path to the archive.
   * \param mount_point the virtual path of the mount point.
   * \param insertion_point mount in the beginning or end of the search path.
   */
  Mount(boost::filesystem::path archive_path, std::string mount_point,
        SearchPathInsertion insertion_point);

  /*!
   * \brief Mounts an archive to a virtual path.
   *
   * If this mount handle is already in use, the previous path is unmounted first.
   *
   * \param archive_path the physical path to the archive.
   * \param mount_point the virtual path of the mount point.
   * \param insertion_point mount in the beginning or end of the search path.
   */
  void mount(boost::filesystem::path archive_path, std::string mount_point,
             SearchPathInsertion insertion_point);

  /*! \brief Releases the mount handle by unmounting the managed path.
   */
  void unmount();

  /*! \brief Returns whether this mount handle is currently guarding a mount point.
   */
  bool isMounted() const;

  /*! \brief The physical path to the mounted archive.
   *
   *  \returns the archive path, empty if the mount handle is currently inactive.
   */
  const boost::filesystem::path& archivePath() const;

  /*! \brief The virtual path of the mount point.
   *  \return the virtual path the archive is mounted to, empty if the mount handle is currently inactive.
   */
  const std::string& mountPath() const;

  /*!
   * Makes sure to unmount the archive, if one is currently mounted.
   */
  ~Mount();

  friend void swap(Mount& m1, Mount& m2);

private:
  /// the path of the mounted archive
  boost::filesystem::path m_archivePath;
  /// the mount target
  std::string m_mountPoint;
};

/*! \brief Standards-compliant implementation of the \c Swappable for Mount.
 */
void swap(Mount& m1, Mount& m2);

/// Possible modes for opening a PhysFS file.
enum class OpenMode {
  Read,   ///< Open file for reading.
  Write,  ///< Open file for writing.
  Append, ///< Open file for writing to the end of the file.
};

/*! \brief Functor closing PhysFS file handles.
 *
 *  It is intended to be used as a standards compliant deleter, e.g. for \c unique_ptr.
 */
struct FileHandleDeleter {
  /*! \brief Closes the passed file handle.
   *  \param file the file handle to be closed.
   */
  void operator()(PHYSFS_File* file) {
    PHYSFS_close(file);
  }
};

/*!
 * \brief RAII file handle for PhysFS.
 *
 * This managed wrapper implies unique ownership of the underlying handle.
 */
using FileHandle =
    std::unique_ptr<PHYSFS_File, FileHandleDeleter>;

/*!
 * \brief Opens a managed PhysFS file.
 * \param path The virtual path of the file.
 * \param mode The mode indicating how to open the file.
 */
FileHandle open(const std::string& path, OpenMode mode);

/*!
 * \brief Opens a raw PhysFS file.
 * \param path The virtual path of the file.
 * \param mode The mode indicating how to open the file.
 */
PHYSFS_File* openRaw(const std::string& path, OpenMode mode);

/*!
 * \brief checks the existence of a file
 * \param path the path of the file to check
 * \return a boolean indicating whether a file exists under \p path
 */
bool exists(const std::string& path);

/*!
 * \brief A Boost.Iostreams device working on PhysFS file handles.
 */
class Device {
public:
  /*! \brief Defines the stream category.
   *
   *  PhysFS streams are seekable, flushable and closable.
   */
  struct category_t : virtual boost::iostreams::device_tag,
                      boost::iostreams::seekable,
                      boost::iostreams::flushable_tag,
                      boost::iostreams::closable_tag {};

  /// The type of chars read from the stream.
  using char_type = char;
  /// The stream category.
  using category = category_t;

  /*!
   * \brief Initializes the PhysFS file device.
   *
   * \param virtual_path The virtual path of the PhysFS file to be opened.
   * \param mode defines how the file should be openend.
   *
   * \note The file handle created in this constructor is only released when
   *  explicitly calling Device::close, due to the way of how Boost expects
   *  devices to function.
   *  A device may be safely copied, as long as Device::close is only called on
   *  one of the copies.
   */
  Device(const std::string& virtual_path, physfs::OpenMode mode);

  /*!
   * \brief Tries to read the requested number \p num of bytes into the \p target.
   * \param[out] target A block of memory that is at least \p num characters long.
   * \param[in] num The number of characters to be read.
   * \return
   * Value    | Description
   * --------:|:---------------------------------------------------------------
   * -1       | If the end of the file has been reached.
   * `n >= 0` | The actual number of characters that have been read into \p target.
   *
   */
  std::streamsize read(char_type* target, std::streamsize num);

  /*!
   * \brief Tries to write the requested number \p num of characters from the \p source into the
   * file.
   * \param[out] source A block of memory that is at least \p num characters long.
   * \param[in] num The number of characters to be written.
   * \return the number of characters that have been written
   *
   */
  std::streamsize write(const char_type* source, std::streamsize num);

  /*!
   * \brief Allows to set the position where the next read or write will happen.
   *
   * Advances the read/write head by \p off characters, returning the new position,
   * where the offset is calculated from:
   *   - the start of the sequence if \p way == ios_base::beg
   *   - the current position if \p way == ios_base::cur
   *   - the end of the sequence if \p way == ios_base::end
   * \param off the amount of characters the read/write head is moved
   * \param way the origin of the seek movement
   * \return the new position
   */
  std::streampos seek(boost::iostreams::stream_offset off, std::ios_base::seekdir way);

  /*!
   * \brief Closes the underlying PhysFS file.
   */
  void close();

  /*!
   * \brief Flushes the underlying PhysFS file.
   * \return a boolean value indicating success
   */
  bool flush();

private:
  PHYSFS_File* m_handle; ///< the underlying file handle
};

/*! \brief A Boost stream accessing PhysFS files.
 */
using Stream = boost::iostreams::stream<Device>;

/*!
 * \brief An PhysFS input stream that is usable by SFML.
 */
class InputStream : public sf::InputStream {
public:
  /*! \brief Creates an input stream from a managed readable file handle.
   *
   *  The input stream takes ownership of the file handle it is passed.
   *  The file handle is closed when this input stream is destroyed.
   *  There is now way of getting ownership back.
   *
   *  \param handle a valid PhysFS file handle
   */
  InputStream(FileHandle handle);

  /*! \brief Opens the file specified by \p virtualPath for reading.
   *  \param virtualPath the path of the file to be read.
   */
  InputStream(const std::string& virtualPath);

  /*! \see <a href="http://www.sfml-dev.org/documentation/latest/classsf_1_1InputStream.php">sf::InputStream</a> */
  sf::Int64 read(void* data, sf::Int64 size) override;

  /*! \see <a href="http://www.sfml-dev.org/documentation/latest/classsf_1_1InputStream.php">sf::InputStream</a> */
  sf::Int64 seek(sf::Int64 position) override;

  /*! \see <a href="http://www.sfml-dev.org/documentation/latest/classsf_1_1InputStream.php">sf::InputStream</a> */
  sf::Int64 tell() override;

  /*! \see <a href="http://www.sfml-dev.org/documentation/latest/classsf_1_1InputStream.php">sf::InputStream</a> */
  sf::Int64 getSize() override;

private:
  /// the underlying file handle
  FileHandle m_handle;
};


}
