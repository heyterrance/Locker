#include <string>
#include <iostream>
#include <fstream>

#include "util.h"

void lockFile(const std::string& path);
void unlockFile(const std::string& key_path, const std::string& path);

int main(int argc, char* argv[]) {
  switch (argc) {
    case 2: lockFile(argv[1]); break;
    case 3: unlockFile(argv[1], argv[2]); break;
    default: printf("Incorrect number of arguments\n"); return argc;
  }

  return 0;
}

void lockFile(const std::string& path) {
  const auto extpos = std::min(path.find_last_of("."), path.size());
  auto slashpos = path.find_last_of("/");
  if (slashpos == std::string::npos) slashpos = 0;
  const std::string fileext = path.substr(extpos);
  const std::string filename = path.substr(slashpos, extpos - slashpos);
  printf("filename: %s, ext: %s\n", filename.c_str(), fileext.c_str());
 
  // Put file into vector.
  std::ifstream file(path, std::ios::in | std::ios::binary);
  printf("Loading file...\n");
  char_vec v = fileToVec(file); 
  file.close();
  printf("File size: %luMB\n", v.size() >> 20);

  // Read file into map.
  char_set s;
  size_t progress = 0;
  for (const auto& c : v) {
    s.insert(c);
    loadBar(progress++, v.size(), "Reading file\t");
  }
  printf("\n");

  char_map m = setToMap(s);
  std::ofstream key_file("./" + filename + ".key",
                         std::ofstream::out | std::ios::binary);
  progress = 0;
  for (const auto& it : m) {
    const char a = it.first;
    const char b = it.second;
    key_file.put(a);
    key_file.put(b);
    loadBar(progress++, m.size(), "Creating key\t");
  }
  printf("\n");
  key_file.close();

  std::ofstream lock_file("./" + filename + ".lock",
                          std::ofstream::out | std::ios::binary);
  progress = 0;
  for (const auto& c : v) {
    lock_file << m[c];
    loadBar(progress++, v.size(), "Locking file\t");
  }
  lock_file.close();
  printf("\n");
}

void unlockFile(const std::string& key_path, const std::string& path) {
  size_t progress = 0;
  char_map m;
  printf("Loading key...\n");
  {
    std::ifstream key_file(key_path, std::ios::in | std::ios::binary);
    char_vec kv = fileToVec(key_file);
    for (auto it = kv.begin(), end = kv.end(); it != end; ++it) {
      const char a = *it;
      const char b = *(++it);
      m[b] = a;
    }
    key_file.close();
  }
 
  std::string out_name = path.substr(0, path.find_last_of(".")); 
  std::ofstream out_file(out_name + ".out",
                         std::ofstream::out | std::ios::binary);
  
  printf("Unlocking file...");
  std::ifstream lock_file(path, std::ios::in | std::ios::binary);
  char_vec v = fileToVec(lock_file);
  for (const char& c : v) {
    out_file << m[c];
    loadBar(progress++, v.size());
  }
  printf("\n");
  out_file.close();
}
