#pragma once

#include <chrono>
#include <iostream>
#include <unordered_set>

#include "command.hxx"
#include "dir_diff.hxx"
#include "repository.hxx"
#include "revision.hxx"

using namespace std;

namespace command {

class Merge: public Command {
  public:
  string name() const override {
    return "merge";
  }
  string description() const override {
    return "Initiate a merge with the currently checked out commit and the specified commit.";
  }
  size_t min_arguments() const override {
    return 1;
  }
  size_t max_arguments() const override {
    return 1;
  }

  int run_inner(vector<string>& arguments) override {
    auto status = repo().status();
    if (!status.empty()) {
      cerr << "Cannot merge with uncommitted files." << endl;
      return 1;
    }

    auto current_revision = repo().current_revision().value();
    auto merge_revision = Revision(arguments[0]);

    if (current_revision == merge_revision) {
      cerr << "Cannot merge current revision with itself." << endl;
      return 1;
    }

    cout << "Merging " << merge_revision.id() << " into " << current_revision.id() << endl;

    auto base_revision = repo().merge_base(current_revision, merge_revision);
    if (!base_revision) {
      cerr << "No merge base found for " << current_revision.id() << " and " << merge_revision.id() << endl;
      return 1;
    }

    cout << "Merge base: " << base_revision.value().id() << endl;

    auto temp_repo_dir_base = repo().checkout_temp_directory(base_revision);
    auto temp_repo_dir_merge = repo().checkout_temp_directory(optional(merge_revision));

    auto repo_dir = repo().dir();
    auto file_statuses_current = dir_diff(*temp_repo_dir_base, repo_dir);
    auto file_statuses_merge = dir_diff(*temp_repo_dir_base, *temp_repo_dir_merge);

    auto move_and_create_directory = [](fs::path from, fs::path to) {
      if (to.has_parent_path()) {
        fs::create_directory(to.parent_path());
      }

      fs::rename(from, to);
    };

    unordered_set<string> conflicts;

    auto move_changed_files = [&](fs::path temp_repo_dir, string path, FileStatus status) {
      switch (status) {
        case Added:
        case Modified: {
          move_and_create_directory(temp_repo_dir / path, repo_dir / path);
          break;
        }
        case Deleted: {
          fs::remove(repo_dir / path);
          break;
        }
      }
    };

    for (auto& entry: file_statuses_merge) {
      auto [path, status] = entry;

      if (file_statuses_current.contains(path)) {
        file_statuses_current.erase(path);
        conflicts.insert(path);
        continue;
      }

      move_changed_files(*temp_repo_dir_merge, path, status);
    }

    for (auto& entry: file_statuses_current) {
      auto [path, status] = entry;

      move_changed_files(repo_dir, path, status);
    }

    for (auto& path: conflicts) {
      auto move_if_exists = [&](fs::path from, fs::path to) {
        if (fs::exists(from) || fs::is_symlink(from)) {
          move_and_create_directory(from, to);
        }
      };

      move_if_exists(*temp_repo_dir_merge / path, repo_dir / (path + "." + merge_revision.id()));
      move_if_exists(*temp_repo_dir_base / path, repo_dir / (path + "." + base_revision->id()));
    }

    if (conflicts.empty()) {
      auto next_revision = repo().next_revision();
      auto commit_message = "Merge " + merge_revision.id() + " into " + current_revision.id();
      auto commit =
          ::Commit(next_revision, current_revision, merge_revision, chrono::system_clock::now(), commit_message);

      repo().create_commit(commit, current_revision);
    } else {
      cerr << "Merge conflict(s) detected:" << endl;

      for (auto& path: conflicts) {
        cerr << "  - " << path << endl;
      }

      repo().write_merge_revision(merge_revision);

      return 1;
    }

    return 0;
  }
};

}
