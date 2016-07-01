#include "physfs.hpp"

#include <physfs.h>

#include <iostream>
#include <stdexcept>

namespace physfs {

IOException::IOException() : IOException(PHYSFS_getLastError()) {}

IOException::IOException(const char* what_arg) : std::ios_base::failure(what_arg) {}

IOException::IOException(const std::string& what_arg) : std::ios_base::failure(what_arg) {}

Initializer::Initializer(const char* argv0) {
  if (!PHYSFS_init(argv0)) {
    throw IOException();
  }
}

Initializer::~Initializer() {
  PHYSFS_deinit();
}

Mount::Mount() {
}

Mount::Mount(boost::filesystem::path archive_path, std::string mount_point,
             SearchPathInsertion insertion_point) {
  mount(archive_path, mount_point, insertion_point);
}

Mount::Mount(Mount&& other) {
  std::swap(m_archivePath, other.m_archivePath);
  std::swap(m_mountPoint, other.m_mountPoint);
  assert(! other.isMounted());
}

Mount& Mount::operator=(Mount&& other) {
  // clear other
  Mount tmp(std::move(other));
  // replace mounted handles
  std::swap(*this, other);
  // tmp now makes sure to unmount the archive previously held by this instance
  return *this;
}

void Mount::mount(boost::filesystem::path archive_path, std::string mount_point,
                  SearchPathInsertion insertion_point) {
  if(isMounted()) {
    unmount();
  }
  m_archivePath = archive_path;
  m_mountPoint = mount_point;
  PHYSFS_mount(archive_path.c_str(),
               mount_point.c_str(),
               insertion_point == SearchPathInsertion::Append ? 1 : 0);
}


void Mount::unmount() {
  if(isMounted()) {
    PHYSFS_removeFromSearchPath(m_archivePath.c_str());
    m_archivePath.clear();
    m_mountPoint.clear();
  }
}

bool Mount::isMounted() const {
  return ! m_archivePath.empty();
}

const boost::filesystem::path& Mount::archivePath() const {
  return m_archivePath;
}

const std::string& Mount::mountPath() const {
  return m_mountPoint;
}

Mount::~Mount() {
  unmount();
}

void swap(Mount& m1, Mount& m2) {
  std::swap(m1.m_mountPoint, m2.m_mountPoint);
  std::swap(m1.m_archivePath, m2.m_archivePath);
}

FileHandle open(const std::string& path, OpenMode mode) {
  return FileHandle(openRaw(path, mode));
}

PHYSFS_File* openRaw(const std::string& path, OpenMode mode) {
  PHYSFS_File* handle{};
  switch (mode) {
  case OpenMode::Read:
    handle = PHYSFS_openRead(path.c_str());
    break;
  case OpenMode::Write:
    handle = PHYSFS_openWrite(path.c_str());
    break;
  case OpenMode::Append:
    handle = PHYSFS_openAppend(path.c_str());
    break;
  }

  if (!handle) {
    throw IOException();
  } else {
    return handle;
  }
}

Device::Device(const std::string& virtual_path, OpenMode mode)
    : m_handle(openRaw(virtual_path, mode)) {}

std::streamsize Device::read(Device::char_type* target, std::streamsize num) {
  if (num > std::numeric_limits<PHYSFS_uint32>::max()) {
    throw std::logic_error("reading of blocks larger than 4 GiB is not supported");
  } else {
    return PHYSFS_read(m_handle, target, sizeof(char_type), static_cast<PHYSFS_uint32>(num));
  }
}

std::streamsize Device::write(const Device::char_type* source, std::streamsize num) {
  if (num > std::numeric_limits<PHYSFS_uint32>::max()) {
    throw std::logic_error("writing of blocks larger than 4 GiB is not supported");
  } else {
    auto ret = PHYSFS_write(m_handle, source, sizeof(char_type), static_cast<PHYSFS_uint32>(num));
    if (ret < 0) {
      // complete failure
      throw IOException();
    } else {
      return ret;
    }
  }
}

std::streampos Device::seek(boost::iostreams::stream_offset off, std::ios_base::seekdir way) {
  PHYSFS_uint64 desired_pos;
  switch (way) {
  case std::ios_base::beg:
    if (off < 0) {
      throw IOException("offset points in front of the beginning");
    }
    desired_pos = static_cast<PHYSFS_uint64>(off);
    break;
  case std::ios_base::cur: {
    PHYSFS_sint64 cur_pos = PHYSFS_tell(m_handle);
    if (cur_pos < 0) {
      throw IOException();
    } else {
      desired_pos = static_cast<PHYSFS_uint64>(cur_pos + off);
    }
    break;
  }
  case std::ios_base::end: {
    PHYSFS_sint64 end_pos = PHYSFS_fileLength(m_handle);
    if (end_pos < 0) {
      throw IOException("seeking from the end of the stream is not possible since the length of "
                        "file cannot be determined");
    } else {
      desired_pos = static_cast<PHYSFS_uint64>(end_pos - off);
    }
    break;
  }
  default:
    throw std::logic_error("invalid argument 'way'");
  }
  if (!PHYSFS_seek(m_handle, desired_pos)) {
    throw IOException();
  } else {
    return static_cast<std::streampos>(desired_pos);
  }
}

void Device::close() {
  PHYSFS_close(m_handle);
}

bool Device::flush() {
  return PHYSFS_flush(m_handle);
}

bool exists(const std::string& path) {
  return PHYSFS_exists(path.c_str());
}

InputStream::InputStream(FileHandle handle)
  : m_handle(std::move(handle)) {}
InputStream::InputStream(const std::string& virtualPath)
  : InputStream(open(virtualPath, OpenMode::Read)) {}

sf::Int64 InputStream::read(void* data, sf::Int64 size) {
  assert(size <= std::numeric_limits<PHYSFS_uint32>::max());
  PHYSFS_sint64 ret = PHYSFS_read(m_handle.get(), data, 1, static_cast<PHYSFS_uint32>(size));
  if(ret >= 0) {
    return ret;
  } else {
    return -1;
  }
}

sf::Int64 InputStream::seek(sf::Int64 position) {
  if(PHYSFS_seek(m_handle.get(), position)) {
    return position;
  } else {
    return -1;
  }
}

sf::Int64 InputStream::tell() {
  return PHYSFS_tell(m_handle.get());
}

sf::Int64 InputStream::getSize() {
  return PHYSFS_fileLength(m_handle.get());
}

}
