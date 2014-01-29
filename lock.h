#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>

#include "util.h"

class Lock {
public:
  int lock(const std::string& file_path);

  int unlock(const std::string& key_path, const std::string& lock_path);

private:
  char_vec loadFile(const std::string& path);
  void setFileName(const std::string& path);
  void createKey();
  void createLock();

private:
  std::string file_name, file_ext;
  char_map m; // key m: char -> char
  char_vec v; // holds entire file.
  char_set s; // holds each char in file.
};
