#pragma once

#include <cassert>
#include <chrono>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../commit.hxx"
#include "command.hxx"
#include "diff.hxx"
#include "dir_diff.hxx"
#include "patch.hxx"
#include "repository.hxx"
#include "revision.hxx"

using namespace std;
namespace fs = std::filesystem;

fs::path create_temp_directory() {
  auto pattern = fs::temp_directory_path() / "lit.XXXXXX";
  return fs::path(mkdtemp((char*)pattern.c_str()));
}

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

namespace command {

class Commit: public Command {
  public:
  string name() const override {
    return "commit";
  }
  string description() const override {
    return "Record changes to the repository.";
  }

  int run(vector<string>& arguments) override {
    if (arguments.size() != 1) {
      cerr << "This command requires exactly one argument." << endl;
      return 1;
    }

    auto repo = Repository();
    auto commit =
        ::Commit(repo.next_revision(), repo.current_revision(), nullopt, chrono::system_clock::now(), arguments[0]);

    cout << "Creating commit " << commit.id() << " '" << commit.message() << "'" << endl;

    auto tmpdir = create_temp_directory();

    auto meta_file_path = tmpdir / commit.revision().meta_filename();
    auto patch_file_path = tmpdir / commit.revision().patch_filename();

    ofstream meta_file(meta_file_path);
    commit.serialize(meta_file);
    meta_file.close();

    auto patch = create_patch(repo.working_copy_dir(), repo.dir(), patch_file_path);
    patch.apply(repo.working_copy_dir());

    fs::rename(meta_file_path, repo.revisions_dir() / commit.revision().meta_filename());
    fs::rename(patch_file_path, repo.revisions_dir() / commit.revision().patch_filename());
    fs::remove_all(tmpdir);

    repo.write_revision(commit.revision());

    return 0;
  }
};

}
