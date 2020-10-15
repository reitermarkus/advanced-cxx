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
#include "fs.hxx"
#include "patch.hxx"
#include "repository.hxx"
#include "revision.hxx"

using namespace std;

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
    auto merge_revision = repo.merge_revision();
    auto commit = ::Commit(next_revision, current_revision, merge_revision, chrono::system_clock::now(), arguments[0]);

    cout << "Creating commit " << commit.id() << " '" << commit.message() << "'" << endl;

    repo.create_commit(commit, current_revision);

    return 0;
  }
};

}
