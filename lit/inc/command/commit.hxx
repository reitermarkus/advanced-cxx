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
    auto commit_message = arguments[0];
    auto revision = repo.next_revision();
    auto commit_date = chrono::system_clock::now();

    cout << "Creating commit " << revision.id() << " '" << commit_message << "'" << endl;

    auto tmpdir = create_temp_directory();

    auto meta_file = tmpdir / revision.meta_filename();
    auto patch_file = tmpdir / revision.patch_filename();

    ofstream meta;
    meta.open(meta_file);

    ofstream patch;
    patch.open(patch_file);

    meta << "Commit: " << revision.id() << endl;
    meta << "Parents:";
    vector<Revision> parent_commit_ids;
    for (auto& parent_revision: parent_commit_ids) {
      meta << " " << parent_revision.id();
    }
    meta << endl;
    meta << "Date: " << chrono::system_clock::to_time_t(commit_date) << endl;
    meta << endl;
    meta << commit_message << endl;

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

    meta.close();
    patch.close();

    SubProcess patch_command("patch");
    patch_command.arg("-t").arg("-s").arg("-p1").arg("-i").arg(patch_file).cwd(repo.working_copy_dir());
    auto [output, status] = patch_command.output();
    assert(status == 0);

    fs::rename(meta_file, repo.revisions_dir() / revision.meta_filename());
    fs::rename(patch_file, repo.revisions_dir() / revision.patch_filename());
    fs::remove_all(tmpdir);

    repo.write_revision(revision);

    return 0;
  }
};

}
