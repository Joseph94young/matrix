#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <time.h>
#include <unistd.h>
#include <cassert>
import <iostream>;
import <vector>;

import file;

constexpr auto kBytes = 1024;

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << "\n";
    return EXIT_FAILURE;
  }
  const std::string path = argv[1];

  struct stat sb2;
  if (lstat(path.c_str(), &sb2) == -1) {
    std::cerr << "lstat: " << ::strerror(errno) << "\n";
    return EXIT_FAILURE;
  }

  struct stat sb = sb2;

  printf("ID of containing device:  [%x,%x]\n", major(sb.st_dev),
         minor(sb.st_dev));

  printf("File type:                ");

  switch (sb.st_mode & S_IFMT) {
    case S_IFBLK:
      printf("block device\n");
      break;
    case S_IFCHR:
      printf("character device\n");
      break;
    case S_IFDIR:
      printf("directory\n");
      break;
    case S_IFIFO:
      printf("FIFO/pipe\n");
      break;
    case S_IFLNK:
      printf("symlink\n");
      break;
    case S_IFREG:
      printf("regular file\n");
      break;
    case S_IFSOCK:
      printf("socket\n");
      break;
    default:
      printf("unknown?\n");
      break;
  }

  printf("I-node number:            %ju\n", (uintmax_t)sb.st_ino);

  printf("Mode:                     %jo (octal)\n", (uintmax_t)sb.st_mode);

  printf("Link count:               %ju\n", (uintmax_t)sb.st_nlink);
  printf("Ownership:                UID=%ju   GID=%ju\n", (uintmax_t)sb.st_uid,
         (uintmax_t)sb.st_gid);

  printf("Preferred I/O block size: %jd bytes\n", (intmax_t)sb.st_blksize);
  printf("File size:                %jd bytes\n", (intmax_t)sb.st_size);
  printf("Blocks allocated:         %jd\n", (intmax_t)sb.st_blocks);

  printf("Last status change:       %s", ctime(&sb.st_ctime));
  printf("Last file access:         %s", ctime(&sb.st_atime));
  printf("Last file modification:   %s", ctime(&sb.st_mtime));

  constexpr auto kBuflen = 64 * kBytes;
  char buf[kBuflen + 1] = "";
  File f(path);
  ::read(f.fd(), buf, kBuflen);
  std::cout << buf << '\n';
  std::cout << getpagesize() << '\n';
  std::vector<char> vec;
  std::cout << vec.capacity() << '\n';
  return EXIT_SUCCESS;
}
