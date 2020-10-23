#include "patch.hxx"

#include <iostream>

#include "diff.hxx"

namespace lit {

Patch Patch::create(const fs::path& dir_a, const fs::path& dir_b, const fs::path& patch_file_path) {
  const auto file_statuses = dir_diff(dir_a, dir_b);

  ofstream patch_file(patch_file_path);

  for (auto& entry: file_statuses) {
    const auto [path, status] = entry;

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

}
