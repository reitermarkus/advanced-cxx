#pragma once

#include <filesystem>
#include <unistd.h>

using namespace std;

namespace fs {

using namespace std::filesystem;

using temp_path = unique_ptr<fs::path, void (*)(fs::path*)>;

temp_path create_temp_directory() {
  auto pattern = temp_directory_path() / "lit.XXXXXX";

  return temp_path(new path(mkdtemp((char*)pattern.c_str())), [](fs::path* path) { fs::remove_all(*path); });
}

}
