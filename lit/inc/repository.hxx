#pragma once

#include <unordered_map>

#include "file_status.hxx"

using namespace std;
namespace fs = std::filesystem;

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

class Repository {
  fs::path path;

  public:
  Repository(fs::path path = fs::current_path()) {
    this->path = path;
  }

  fs::path dir() {
    return this->path;
  }

  fs::path lit_dir() {
    return this->path / ".lit";
  }

  fs::path revisions_dir() {
    return lit_dir() / "revisions";
  }

  fs::path working_copy_dir() {
    return lit_dir() / "working-copy";
  }

  void initialize() {
    fs::create_directory(lit_dir());
    fs::create_directory(working_copy_dir());
  }

  bool is_initialized() {
    return fs::is_directory(lit_dir());
  }

  unordered_map<string, FileStatus> status() {
    unordered_map<string, FileStatus> file_statuses;

    auto dir_parts = count_path_parts(dir());
    auto working_copy_dir_parts = dir_parts + 2;

    auto status = [&](fs::directory_entry entry) {
      auto path = entry.path();

      if (is_directory(path)) {
        // Only track changes to files.
        return;
      }

      auto new_path = strip_path_prefix_parts(path, dir_parts);

      auto comparison_path = working_copy_dir() / new_path;

      auto existed_previously = exists(comparison_path);
      if (existed_previously) {
        file_statuses[string(new_path)] = Modified;
      } else {
        file_statuses[string(new_path)] = Added;
      }
    };

    for (auto& entry: fs::directory_iterator(dir())) {
      auto path = entry.path();

      if (path == lit_dir()) {
        continue;
      } else if (entry.is_directory()) {
        for (auto& recursive_entry: fs::recursive_directory_iterator(path)) {
          status(recursive_entry);
        }
      } else {
        status(entry);
      }
    }

    for (auto& entry: fs::recursive_directory_iterator(working_copy_dir())) {
      auto path = entry.path();

      auto new_path = strip_path_prefix_parts(path, working_copy_dir_parts);
      auto new_path_string = string(new_path);

      if (file_statuses.contains(new_path_string)) {
        continue;
      }

      auto was_deleted = !exists(dir() / new_path);
      if (was_deleted) {
        file_statuses[new_path_string] = Deleted;
      }
    }

    return file_statuses;
  }
};
