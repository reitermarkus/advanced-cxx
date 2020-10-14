#pragma once

#include <numeric>
#include <unordered_map>

#include "file_status.hxx"

size_t count_path_parts(fs::path path) {
  return accumulate(path.begin(), path.end(), 0, [](size_t acc, fs::path _) { return acc + 1; });
}

fs::path strip_path_prefix_parts(fs::path path, size_t n) {
  auto begin = path.begin();
  advance(begin, n);
  fs::path init = *begin;
  advance(begin, 1);
  return reduce(begin, path.end(), init, divides<fs::path>());
}

unordered_map<string, FileStatus> dir_diff(fs::path dir_a, fs::path dir_b) {
  unordered_map<string, FileStatus> file_statuses;

  auto dir_a_parts = count_path_parts(dir_a);
  auto dir_b_parts = count_path_parts(dir_b);

  auto status_b = [&](fs::directory_entry entry) {
    auto path = entry.path();

    if (is_directory(path)) {
      // Only track changes to files.
      return;
    }

    auto new_path = strip_path_prefix_parts(path, dir_b_parts);

    auto comparison_path = dir_a / new_path;

    auto existed_previously = exists(comparison_path);
    if (existed_previously) {
      auto diff = Diff(path, comparison_path);

      if (diff.is_changed()) {
        file_statuses[string(new_path)] = Modified;
      }
    } else {
      file_statuses[string(new_path)] = Added;
    }
  };

  for (auto& entry: fs::directory_iterator(dir_b)) {
    auto path = entry.path();

    if (path == dir_b / ".lit") {
      continue;
    }
    if (entry.is_directory()) {
      for (auto& recursive_entry: fs::recursive_directory_iterator(path)) {
        status_b(recursive_entry);
      }
    } else {
      status_b(entry);
    }
  }

  auto status_a = [&](fs::directory_entry entry) {
    auto path = entry.path();

    auto new_path = strip_path_prefix_parts(path, dir_a_parts);
    auto new_path_string = string(new_path);

    if (file_statuses.contains(new_path_string)) {
      return;
    }

    auto was_deleted = !exists(dir_b / new_path);
    if (was_deleted) {
      file_statuses[new_path_string] = Deleted;
    }
  };

  for (auto& entry: fs::directory_iterator(dir_a)) {
    auto path = entry.path();

    if (path == dir_a / ".lit") {
      continue;
    }
    if (entry.is_directory()) {
      for (auto& recursive_entry: fs::recursive_directory_iterator(path)) {
        status_a(recursive_entry);
      }
    } else {
      status_a(entry);
    }
  }

  return file_statuses;
}
