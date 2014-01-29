#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>

#include "util.h"
#include "lock.h"

int lockFile(const std::string& path);
int unlockFile(const std::string& key_path, const std::string& path);

int main(int argc, char* argv[]) {
  Lock l;
  switch (argc) {
    case 2: return l.lock(argv[1]);
    case 3: return l.unlock(argv[1], argv[2]);
    default:
      printf("usage: %s target_file\n", argv[0]);
      printf("       %s file.key file.lock\n", argv[0]);
      return 64;
  }

  return 0;
}
