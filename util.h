#include <time.h>
#include <stdlib.h>

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>

#pragma once

typedef std::unordered_map<char, char>  char_map;
typedef std::unordered_set<char>        char_set;
typedef std::vector<char>               char_vec;

inline char_vec fileToVec(std::ifstream& ifs) {
  // Get filesize
  const auto fstart = ifs.tellg();
  ifs.seekg(0, std::ifstream::end);
  const auto fend = ifs.tellg();
  ifs.clear();
  ifs.seekg(0, std::ios::beg); // move to beginning of stream.
  const auto file_size = fend - fstart;

  char_vec v(file_size);
  ifs.read(&v[0], file_size);
  return v ;
}

inline char_map setToMap(const char_set& s) {
  char_vec v(s.begin(), s.end());
  srand((unsigned) time(NULL));
  std::random_shuffle(v.begin(), v.end(), [](int i){ return rand() % i; });

  char_map m;
  for (char c: s) {
    m[c] = v.back();
    v.pop_back();
  }
  return m;
}

inline void loadBar(size_t x, size_t n, const char* text = "") {
  const size_t w = 40lu;
  if ((x != n) and (x % ((n/100) + 1) != 0)) return;
  float rat = x / (float) n;
  int c = rat * w;
  std::cout << text << " [";
  for (int i = 0; i < c; ++i) std::cout << "#";
  for (int i = c; i < w; ++i) std::cout << " ";
  std::cout << "]\r" << std::flush;
}
