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
  size_t min_arguments() const override {
    return 1;
  }
  size_t max_arguments() const override {
    return 1;
  }

  int run_inner(vector<string>&& arguments) override {
    const auto next_revision = repo().next_revision();
    const auto current_revision = repo().current_revision();
    const auto merge_revision = repo().merge_revision();
    const auto commit = ::Commit(next_revision, current_revision, merge_revision, chrono::system_clock::now(), arguments[0]);

    cout << "Creating commit " << commit.id() << " '" << commit.message() << "'" << endl;

    repo().create_commit(commit, current_revision);

    return 0;
  }
};

}
