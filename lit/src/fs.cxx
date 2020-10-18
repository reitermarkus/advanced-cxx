#include "fs.hxx"

namespace fs {

temp_path create_temp_directory() {
  auto pattern = temp_directory_path() / "lit.XXXXXX";

  return temp_path(new path(mkdtemp((char*)pattern.c_str())), [](fs::path* path) { fs::remove_all(*path); });
}

repository_iterator begin(repository_iterator it) noexcept {
  return it;
}

repository_iterator end(const repository_iterator&) noexcept {
  return repository_iterator();
}

recursive_repository_iterator begin(recursive_repository_iterator it) noexcept {
  return it;
}

recursive_repository_iterator end(const recursive_repository_iterator&) noexcept {
  return recursive_repository_iterator();
}

}
