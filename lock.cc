#include "lock.h"

int Lock::lock(const std::string& file_path) {
  setFileName(file_path);

  printf("Loading file...\n");
  v = loadFile(file_path);

  printf("File size: %luMB\n", v.size() >> 20);
  
  // Get password. (Currently unused)
  const std::string password(getpass("Password: "));
  if (password.size() != 0 && password.compare(getpass("Confirm: ")) != 0) {
    printf("Passwords do not match\n");
    return 8;
  }

  createKey();
  createLock();
  return 0;
}

char_vec Lock::loadFile(const std::string& path) {
  std::ifstream file(path, std::ios::in | std::ios::binary);
  char_vec v = fileToVec(file); 
  file.close();
  return v;
}

void Lock::setFileName(const std::string& path) {
  const auto extpos = std::min(path.find_last_of("."), path.size());
  auto slashpos = path.find_last_of("/");
  if (slashpos == std::string::npos) slashpos = 0;
  else ++slashpos;

  file_ext  = path.substr(extpos);
  file_name = path.substr(slashpos, extpos - slashpos);
}

void Lock::createKey() {
  // Read file into map.
  size_t progress = 0;

  for (const char& c : v) {
    s.insert(c);
    loadBar(progress++, v.size(), "Reading file\t");
  }
  printf("\n");
  m = setToMap(s);

  std::ofstream key_file("./" + file_name + ".key",
                         std::ofstream::out | std::ios::binary);
  char fname[256];
  // Put filename in key.
  strncpy(fname, file_name.c_str(), 256);
  strncpy(fname + file_name.size(), file_ext.c_str(), 256 - file_name.size());
  for (size_t i = 0; i < 256; ++i)
    key_file.put(fname[i]);

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
}

void Lock::createLock() {
  std::ofstream lock_file("./" + file_name + ".lock",
                          std::ofstream::out | std::ios::binary);
  size_t progress = 0;
  for (const auto& c : v) {
    lock_file << m[c];
    loadBar(progress++, v.size(), "Locking file\t");
  }
  lock_file.close();
  printf("\n");
}
  
int Lock::unlock(const std::string& key_path, const std::string& lock_path) {
  char fname[256];
  printf("Loading key\n");
  {
    std::ifstream key_file(key_path, std::ios::in | std::ios::binary);
    const char_vec kv = fileToVec(key_file);
    for (size_t i = 0; i < kv.size(); ++i) {
      if (i < 256) {
        fname[i] = kv[i]; 
        continue;
      } else {
        const char a = kv[i];
        const char b = kv[++i];
        m[b] = a;
      }
    }
    key_file.close();
  }
  
  file_name = fname; // includes extension.
  const std::string password(getpass("Password: "));
  setFileName(lock_path);
  std::cout << "Out filename: " << file_name << '\n';
  std::ofstream out_file(file_name, std::ofstream::out | std::ios::binary);
  
  std::ifstream lock_file(lock_path, std::ios::in | std::ios::binary);
  char_vec v = fileToVec(lock_file);
  size_t progress = 0;
  for (const char& c : v) {
    out_file << m[c];
    loadBar(progress++, v.size(), "Unlocking file...");
  }
  printf("\n");
  out_file.close();
  return 0;
}
