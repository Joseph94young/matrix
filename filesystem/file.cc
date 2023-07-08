export module file;

import <filesystem>;
import <iostream>;
import <optional>;
import <string>;
import <utility>;
import <vector>;

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cassert>

export class File {

 public:
  File() noexcept : fd_(-1) {}

  explicit File(int fd);

  explicit File(const char* name, int flags = O_RDONLY, mode_t mode = 0644)
      : fd_(::open(name, flags, mode)), iovec_(seekHole()) {
    assert(fd_ != -1);
    ::lstat(name, &statbuf_);
    size_t fsize = statbuf_.st_size;
    size_t kPageSize = ::getpagesize() * 1024;
    buffer_.resize((fsize < kPageSize) ? fsize : kPageSize);
  }

  explicit File(const std::string& name, int flags = O_RDONLY,
                mode_t mode = 0644)
      : File(name.c_str(), flags, mode) {}

  explicit File(
      const std::filesystem::path& name, int flags = O_RDONLY,
      std::filesystem::perms perm = std::filesystem::perms::owner_read |
                                    std::filesystem::perms::owner_write |
                                    std::filesystem::perms::group_read |
                                    std::filesystem::perms::others_read)
      : File(
            name.c_str(), flags,
            static_cast<std::underlying_type_t<std::filesystem::perms>>(perm)) {
  }

  ~File() { ::close(fd_); }

  [[nodiscard]] int fd() const { return fd_; }

  // explicit operator bool() const { return fd_ != -1; }

  // void Close();

  // File(File&&) noexcept;

  // File& operator=(File&&) noexcept;

  // void Lock(bool exclusive = true);

  // bool TryLock(bool exclusive = true);

  // void UnLock();

  char* begin() {
    return const_cast<char*>(static_cast<const File&>(*this).begin());
  }

  const char* begin() const { return &*buffer_.begin(); }

 private:
  bool isRegular() const { return S_ISREG(statbuf_.st_mode); }

  bool isDirectory() const { return S_ISDIR(statbuf_.st_mode); }

  bool isCharacter() const { return S_ISCHR(statbuf_.st_mode); }

  bool isBlock() const { return S_ISBLK(statbuf_.st_mode); }

  bool isFifo() const { return S_ISFIFO(statbuf_.st_mode); }

  bool isSymlink() const { return S_ISLNK(statbuf_.st_mode); }

  bool isSock() const { return S_ISSOCK(statbuf_.st_mode); }

  std::optional<std::vector<std::pair<off_t, off_t>>> seekHole() const {
    off_t beg = 0;
    off_t end = 0;
    std::vector<std::pair<off_t, off_t>> vec{};
    while (true) {
      if (beg = ::lseek(fd_, end, SEEK_DATA); beg == -1) {
        if (errno != ENXIO) {
          std::cerr << "Seek data failed " << ::strerror(errno)
                    << ", errno: " << errno << '\n';
        }
        break;
      }
      if (end = ::lseek(fd_, beg, SEEK_HOLE); end == -1) {
        if (errno != ENXIO) {
          std::cerr << "Seek hole failed " << ::strerror(errno)
                    << ", errno: " << errno << '\n';
        }
        break;
      }
      vec.emplace_back(beg, end);
    }
    if (vec.empty()) {
      return std::nullopt;
    }

    return vec;
  }

 private:
  int fd_;
  struct ::stat statbuf_;
  std::optional<std::vector<std::pair<off_t, off_t>>> iovec_;
  std::vector<char> buffer_;
  size_t readerIndex_;
  size_t writerIndex_;
};