#pragma once

#include <filesystem>
#include <unistd.h>

using namespace std;

namespace fs {

using namespace std::filesystem;

fs::path create_temp_directory() {
  auto pattern = temp_directory_path() / "lit.XXXXXX";
  return path(mkdtemp((char*)pattern.c_str()));
}

}
