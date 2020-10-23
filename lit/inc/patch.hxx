#pragma once

#include <cassert>
#include <filesystem>
#include <fstream>

#include "dir_diff.hxx"
#include "file_status.hxx"
#include "fs.hxx"
#include "sub_process.hxx"

using namespace std;

class Patch {
  fs::path path_;

  void patch(const fs::path& dir, bool revert) const {
    SubProcess command("patch");

    command.arg("-d").arg(dir).arg("-t").arg("-s").arg("-p1").arg("-i").arg(this->path_);

    if (revert) {
      command.arg("-R");
    }

    const auto [output, status] = command.output();
    assert(status == 0);
  }

  public:
  Patch(fs::path path): path_(path) {}

  const fs::path& path() const {
    return this->path_;
  }

  void apply(const fs::path& dir) const {
    patch(dir, false);
  }

  void revert(const fs::path& dir) const {
    patch(dir, true);
  }

  static Patch create(const fs::path& dir_a, const fs::path& dir_b, const fs::path& patch_file_path);
};
