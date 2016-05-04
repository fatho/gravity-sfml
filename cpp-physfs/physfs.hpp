#ifndef PHYSFS_H
#define PHYSFS_H

#include <physfs.h>
#include <memory>
#include <string>
#include <streambuf>

#include <SFML/System/InputStream.hpp>
#undef __GNUC__
#include <boost/filesystem/path.hpp>
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/stream.hpp>
#define __GNUC__

namespace physfs {

/**
 * @brief A general exception type wrapping all errors originating from the PhysFS library.
 */
struct IOException : std::ios_base::failure {
  IOException();
  explicit IOException(const char* what_arg);
  explicit IOException(const std::string& what_arg);
};

/**
 * @brief Initializes PhysFS in the constructor and deinitializes in the
 * destructor.
 */
class Initializer {
public:
  /// Initializes PhysFS
  explicit Initializer(const char* argv0);

  /// Deinitializes PhysFS
  ~Initializer();
};

/**
 * @brief Archives can be mounted in the beginning or the end of the
 */
enum SearchPathInsertion {
  append, ///< The archive is added to the end of the search path.
  prepend ///< The archive is added to the front of the search path.
};

/**
 * @brief An RAII mount of a PhysFS archive.
 */
class Mount {
public:
  /** Initializes mount handle without mounting.
   */
  Mount();

  Mount(const Mount&) = delete;
  Mount& operator=(const Mount&) = delete;

  Mount(Mount&& other);
  Mount& operator=(Mount&&);

  /**
   * @brief Mounts an archive to a virtual path.
   * @param archive_path The physical path to the archive.
   * @param mount_point The virtual path of the mount point.
   * @param insertion_point Mount in the beginning or end of the search path.
   */
  Mount(boost::filesystem::path archive_path, std::string mount_point,
        SearchPathInsertion insertion_point);

  /**
   * @brief Mounts an archive to a virtual path.
   * @param archive_path The physical path to the archive.
   * @param mount_point The virtual path of the mount point.
   * @param insertion_point Mount in the beginning or end of the search path.
   */
  void mount(boost::filesystem::path archive_path, std::string mount_point,
             SearchPathInsertion insertion_point);


  void unmount();

  bool isMounted() const;

  /// @returns The physical path to the archive.
  const boost::filesystem::path& archivePath() const;

  /// @returns The virtual path of the mount point.
  const std::string& mountPath() const;

  /**
   * Unmounts the archive.
   */
  ~Mount();

private:
  boost::filesystem::path m_archivePath;
  std::string m_mountPoint;
};

/// Possible modes for opening a PhysFS file.
enum class OpenMode {
  Read,   ///< Open file for reading.
  Write,  ///< Open file for writing.
  Append, ///< Open file for writing to the end of the file.
};

struct FileHandleDeleter {
  void operator()(PHYSFS_File* file) {
    PHYSFS_close(file);
  }
};

/**
 * @brief RAII file handle for PhysFS.
 */
using FileHandle =
    std::unique_ptr<PHYSFS_File, FileHandleDeleter>;

/**
 * @brief Opens a managed PhysFS file.
 * @param path The virtual path of the file.
 * @param mode The mode indicating how to open the file.
 */
FileHandle open(const std::string& path, OpenMode mode);

/**
 * @brief Opens a raw PhysFS file.
 * @param path The virtual path of the file.
 * @param mode The mode indicating how to open the file.
 */
PHYSFS_File* openRaw(const std::string& path, OpenMode mode);

/**
 * @brief checks the existence of a file
 * @param path the path of the file to check
 * @return true if the file \p fname exists, false otherwise
 */
bool exists(const std::string& fname);

/**
 * @brief A Boost.Iostreams device working on PhysFS file handles.
 */
class Device {
public:
  struct category_t : virtual boost::iostreams::device_tag,
                      boost::iostreams::seekable,
                      boost::iostreams::flushable_tag,
                      boost::iostreams::closable_tag {};

  using char_type = char;
  using category = category_t;

  /**
   * @brief Initializes the PhysFS file device.
   * @param handle A reference to a PhysFS file handle of an already opened file.
   * The file handle must outlive this physfs_device instance.
   */
  Device(const std::string& virtual_path, physfs::OpenMode mode);

  /**
   * @brief Tries to read the requested number \p num of bytes into the \p target.
   * @param[out] target A block of memory that is at least \p num characters long.
   * @param[in] num The number of characters to be read.
   * @return
   * Value    | Description
   * --------:|:---------------------------------------------------------------
   * -1       | If the end of the file has been reached.
   * `n >= 0` | The actual number of characters that have been read into \p target.
   *
   */
  std::streamsize read(char_type* target, std::streamsize num);

  /**
   * @brief Tries to write the requested number \p num of characters from the \p source into the
   * file.
   * @param[out] source A block of memory that is at least \p num characters long.
   * @param[in] num The number of characters to be written.
   * @return the number of characters that have been written
   *
   */
  std::streamsize write(const char_type* source, std::streamsize num);

  /**
   * @brief Allows to set the position where the next read or write will happen.
   *
   * Advances the read/write head by \p off characters, returning the new position,
   * where the offset is calculated from:
   *   - the start of the sequence if \p way == ios_base::beg
   *   - the current position if \p way == ios_base::cur
   *   - the end of the sequence if \p way == ios_base::end
   * @param off the amount of characters the read/write head is moved
   * @param way the origin of the seek movement
   * @return the new position
   */
  std::streampos seek(boost::iostreams::stream_offset off, std::ios_base::seekdir way);

  /**
   * @brief Closes the underlying PhysFS file.
   */
  void close();

  /**
   * @brief Flushes the underlying PhysFS file.
   * @return a boolean value indicating success
   */
  bool flush();

private:
  PHYSFS_File* m_handle; ///< the underlying file handle
};

using Stream = boost::iostreams::stream<Device>;

/**
 * @brief An PhysFS input stream that is usable by SFML.
 */
class InputStream : public sf::InputStream {
public:
  InputStream(FileHandle handle);
  InputStream(const std::string& virtualPath);

  sf::Int64 read(void* data, sf::Int64 size) override;

  sf::Int64 seek(sf::Int64 position) override;

  sf::Int64 tell() override;

  sf::Int64 getSize() override;

private:
  FileHandle m_handle;
};


}

#endif // PHYSFS_H
