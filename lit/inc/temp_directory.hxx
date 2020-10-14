#pragma once

#include <filesystem>
#include <unistd.h>

using namespace std;
namespace fs = std::filesystem;

fs::path create_temp_directory() {
  auto pattern = fs::temp_directory_path() / "lit.XXXXXX";
  return fs::path(mkdtemp((char*)pattern.c_str()));
}
