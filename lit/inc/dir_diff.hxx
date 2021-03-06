#pragma once

#include <unordered_map>

#include "diff.hxx"
#include "file_status.hxx"
#include "fs.hxx"

namespace lit {

unordered_map<string, FileStatus> dir_diff(const fs::path& dir_a, const fs::path& dir_b) {
  unordered_map<string, FileStatus> file_statuses;

  const auto dir_a_parts = fs::count_path_parts(dir_a);
  const auto dir_b_parts = fs::count_path_parts(dir_b);

  const auto status_b = [&](const fs::directory_entry& entry) {
    const auto& path = entry.path();
    const auto new_path = fs::strip_path_prefix_parts(path, dir_b_parts);

    const auto comparison_path = dir_a / new_path;

    const auto existed_previously = exists(comparison_path);
    if (existed_previously) {
      const auto diff = Diff(path, comparison_path);

      if (diff.is_changed()) {
        file_statuses[string(new_path)] = Modified;
      }
    } else {
      file_statuses[string(new_path)] = Added;
    }
  };

  for (const auto& entry: fs::recursive_repository_iterator(dir_b)) {
    status_b(entry);
  }

  const auto status_a = [&](const fs::directory_entry& entry) {
    const auto& path = entry.path();
    const auto new_path = fs::strip_path_prefix_parts(path, dir_a_parts);

    if (file_statuses.find(new_path) != file_statuses.end()) {
      return;
    }

    const auto was_deleted = !exists(dir_b / new_path);
    if (was_deleted) {
      file_statuses[string(new_path)] = Deleted;
    }
  };

  for (const auto& entry: fs::recursive_repository_iterator(dir_a)) {
    status_a(entry);
  }

  return file_statuses;
}

} // namespace lit
