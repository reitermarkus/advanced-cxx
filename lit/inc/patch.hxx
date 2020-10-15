#pragma once

#include <cassert>
#include <filesystem>
#include <fstream>

#include "fs.hxx"
#include "sub_process.hxx"

using namespace std;

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

Patch create_patch(fs::path dir_a, fs::path dir_b, fs::path patch_file_path) {
  auto file_statuses = dir_diff(dir_a, dir_b);

  ofstream patch_file(patch_file_path);

  for (auto& entry: file_statuses) {
    auto [path, status] = entry;

    auto path_a = dir_a / path;
    auto label_a = "a/" + path;
    auto path_b = dir_b / path;
    auto label_b = "b/" + path;

    switch (status) {
      case Added: {
        path_a = "/dev/null";
        label_a = "/dev/null";
        break;
      }
      case Modified: {
        break;
      }
      case Deleted: {
        path_b = "/dev/null";
        label_b = "/dev/null";
        break;
      }
    }

    auto diff = Diff(path_a, path_b, label_a, label_b);
    patch_file << diff.output() << endl;
  }

  return Patch(patch_file_path);
}
