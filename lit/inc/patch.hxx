#pragma once

#include <cassert>
#include <filesystem>

#include "sub_process.hxx"

using namespace std;
namespace fs = std::filesystem;

class Patch {
  fs::path dir_;
  fs::path path_;

  void patch(bool revert) {
    SubProcess command("patch");
    command.arg("-t").arg("-s").arg("-p1").arg("-i").arg(this->path_).cwd(this->dir_);

    if (revert) {
      command.arg("-R");
    }

    auto [output, status] = command.output();
    assert(status == 0);
  }

  public:
  Patch(fs::path dir, fs::path path): dir_(dir), path_(path) {}

  void apply() {
    patch(false);
  }

  void revert() {
    patch(true);
  }
};
