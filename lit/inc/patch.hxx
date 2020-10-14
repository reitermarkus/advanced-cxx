#pragma once

#include <cassert>
#include <filesystem>

#include "sub_process.hxx"

using namespace std;
namespace fs = std::filesystem;

class Patch {
  fs::path path_;

  void patch(fs::path dir, bool revert) const {
    SubProcess command("patch");
    command.arg("-t").arg("-s").arg("-p1").arg("-i").arg(this->path_).cwd(dir);

    if (revert) {
      command.arg("-R");
    }

    auto [output, status] = command.output();
    assert(status == 0);
  }

  public:
  Patch(fs::path path): path_(path) {}

  const fs::path& path() const {
    return this->path_;
  }

  void apply(fs::path dir) const {
    patch(dir, false);
  }

  void revert(fs::path dir) const {
    patch(dir, true);
  }
};
