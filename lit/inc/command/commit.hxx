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

#include "command.hxx"
#include "../commit.hxx"
#include "diff.hxx"
#include "repository.hxx"
#include "revision.hxx"

using namespace std;
namespace fs = std::filesystem;

fs::path create_temp_directory() {
  auto pattern = fs::temp_directory_path() / "lit.XXXXXX";
  return fs::path(mkdtemp((char*)pattern.c_str()));
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

    auto meta_file = tmpdir / commit.revision().meta_filename();
    auto patch_file = tmpdir / commit.revision().patch_filename();

    ofstream meta(meta_file);
    commit.serialize(meta);
    meta.close();

    ofstream patch(patch_file);

    auto file_statuses = repo.status();

    for (auto& entry: file_statuses) {
      auto [path, status] = entry;

      auto path_a = repo.working_copy_dir() / path;
      auto label_a = "a/" + path;
      auto path_b = repo.dir() / path;
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
      patch << diff.output() << endl;
    }

    patch.close();

    SubProcess patch_command("patch");
    patch_command.arg("-t").arg("-s").arg("-p1").arg("-i").arg(patch_file).cwd(repo.working_copy_dir());
    auto [output, status] = patch_command.output();
    assert(status == 0);

    fs::rename(meta_file, repo.revisions_dir() / commit.revision().meta_filename());
    fs::rename(patch_file, repo.revisions_dir() / commit.revision().patch_filename());
    fs::remove_all(tmpdir);

    repo.write_revision(commit.revision());

    return 0;
  }
};

}
