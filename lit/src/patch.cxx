#include "patch.hxx"

#include "diff.hxx"

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
