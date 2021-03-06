#pragma once

#include <cassert>
#include <filesystem>
#include <fstream>

#include "dir_diff.hxx"
#include "file_status.hxx"
#include "fs.hxx"
#include "sub_process.hxx"

namespace lit {

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
  explicit Patch(fs::path path): path_(move(path)) {}

  [[nodiscard]] const fs::path& path() const {
    return this->path_;
  }

  void apply(const fs::path& dir) const {
    patch(dir, false);
  }

  void revert(const fs::path& dir) const {
    patch(dir, true);
  }

  static Patch create(const fs::path& dir_a, const fs::path& dir_b, const fs::path& patch_file_path) {
    const auto file_statuses = dir_diff(dir_a, dir_b);

    ofstream patch_file(patch_file_path);

    for (const auto& entry: file_statuses) {
      const auto& [path, status] = entry;

      const auto path_a = dir_a / path;
      auto label_a = "a/" + path;
      const auto path_b = dir_b / path;
      auto label_b = "b/" + path;

      switch (status) {
        case Added: {
          label_a = "/dev/null";
          break;
        }
        case Modified: {
          break;
        }
        case Deleted: {
          label_b = "/dev/null";
          break;
        }
      }

      const auto diff = Diff(path_a, path_b, label_a, label_b);
      patch_file << diff.output() << endl;
    }

    return Patch(patch_file_path);
  }
};

} // namespace lit
