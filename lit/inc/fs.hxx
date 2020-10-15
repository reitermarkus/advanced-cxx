#pragma once

#include <filesystem>
#include <optional>
#include <unistd.h>

using namespace std;

namespace fs {

using namespace std::filesystem;

using temp_path = unique_ptr<fs::path, void (*)(fs::path*)>;

temp_path create_temp_directory() {
  auto pattern = temp_directory_path() / "lit.XXXXXX";

  return temp_path(new path(mkdtemp((char*)pattern.c_str())), [](fs::path* path) { fs::remove_all(*path); });
}

inline bool is_lit(const path& p) {
  return p.filename() == ".lit";
}

/// Iterator for iterating over all top-level directories and files in a repository.
class repository_iterator: public directory_iterator {
  public:
  repository_iterator(): directory_iterator() {}
  repository_iterator(const path& p): directory_iterator(p) {
    skip_if_lit();
  }

  void skip_if_lit() {
    if (*this != end(*this) && is_lit((**this).path())) {
      ++(*this);
    }
  }

  repository_iterator& operator++() {
    directory_iterator::operator++();

    skip_if_lit();

    return *this;
  }
};

}
