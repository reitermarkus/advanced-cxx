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

class recursive_repository_iterator: public repository_iterator {
  bool ignore_directories = true;
  shared_ptr<recursive_repository_iterator> rec_it;

  void skip_directories() {
    if (ignore_directories) {
      while (*this != fs::end(*this)) {
        auto direntry = (**this);

        if (!direntry.is_directory()) {
          break;
        } else {
          ++(*this);
        }
      }
    }
  }

  public:
  recursive_repository_iterator(const recursive_repository_iterator&) = default;
  recursive_repository_iterator() noexcept {}
  recursive_repository_iterator(const path& p, bool ignore_directories = true): repository_iterator(p) {
    this->ignore_directories = ignore_directories;
    skip_directories();
  }

  recursive_repository_iterator& operator++() {
    auto entry = repository_iterator::operator*();

    if (rec_it) {
      ++(*rec_it);

      if (*rec_it == fs::end(*rec_it)) {
        rec_it.reset();
      }
    } else {
      if (entry.is_directory()) {
        auto temp_rec_it = shared_ptr<recursive_repository_iterator>(new recursive_repository_iterator(entry.path()));
        if (*temp_rec_it != fs::end(*temp_rec_it)) {
          rec_it = temp_rec_it;
        }
      }
    }

    if (!rec_it) {
      repository_iterator::operator++();
    }

    skip_directories();

    return *this;
  }

  reference operator*() const {
    return rec_it ? **rec_it : repository_iterator::operator*();
  }

  bool operator==(const recursive_repository_iterator& other) const {
    if (bool(rec_it) != bool(other.rec_it)) {
      return false;
    }

    if (rec_it && other.rec_it && *rec_it != *other.rec_it) {
      return false;
    }

    return (const repository_iterator&)(*this) == (const repository_iterator&)(other);
  }

  bool operator!=(const recursive_repository_iterator& other) const {
    return !(*this == other);
  }
};

recursive_repository_iterator begin(recursive_repository_iterator it) noexcept {
  return it;
}

recursive_repository_iterator end(const recursive_repository_iterator&) noexcept {
  return recursive_repository_iterator();
}

}
