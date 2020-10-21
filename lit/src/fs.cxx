#include "fs.hxx"

#include <numeric>

namespace fs {

size_t count_path_parts(fs::path path) {
  return accumulate(path.begin(), path.end(), 0, [](size_t acc, fs::path _) { return acc + 1; });
}

path strip_path_prefix_parts(path p, size_t n) {
  auto begin = p.begin();
  advance(begin, n);
  path init = *begin;
  advance(begin, 1);
  return reduce(begin, p.end(), init, divides<path>());
}

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
