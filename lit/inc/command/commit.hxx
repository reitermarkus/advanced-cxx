#pragma once

#include <cassert>
#include <chrono>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "../commit.hxx"
#include "command.hxx"
#include "diff.hxx"
#include "dir_diff.hxx"
#include "patch.hxx"
#include "repository.hxx"
#include "revision.hxx"
#include "temp_directory.hxx"

using namespace std;
namespace fs = std::filesystem;

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
    auto next_revision = repo.next_revision();
    auto current_revision = repo.current_revision();
    auto commit = ::Commit(next_revision, current_revision, nullopt, chrono::system_clock::now(), arguments[0]);

    cout << "Creating commit " << commit.id() << " '" << commit.message() << "'" << endl;

    auto tmpdir = create_temp_directory();

    auto meta_file_path = tmpdir / commit.revision().meta_filename();
    auto patch_file_path = tmpdir / commit.revision().patch_filename();

    ofstream meta_file(meta_file_path);
    commit.serialize(meta_file);
    meta_file.close();

    auto temp_repo_dir = repo.checkout_temp_directory(current_revision);
    auto patch = create_patch(temp_repo_dir, repo.dir(), patch_file_path);

    fs::rename(meta_file_path, repo.revisions_dir() / commit.revision().meta_filename());
    fs::rename(patch_file_path, repo.revisions_dir() / commit.revision().patch_filename());
    fs::remove_all(tmpdir);
    fs::remove_all(temp_repo_dir);

    repo.write_revision(commit.revision());

    return 0;
  }
};

}
