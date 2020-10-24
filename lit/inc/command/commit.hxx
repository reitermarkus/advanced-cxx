#pragma once

#include <cassert>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>

#ifndef LIT_COMMAND_HXX
#include "command.hxx"
#endif

#include "../commit.hxx"
#include "diff.hxx"
#include "dir_diff.hxx"
#include "fs.hxx"
#include "patch.hxx"
#include "repository.hxx"
#include "revision.hxx"

namespace lit {

using namespace std;

class CommitCommand: public Command {
  public:
  [[nodiscard]] string description() const override {
    return "Record changes to the repository.";
  }
  [[nodiscard]] size_t min_arguments() const override {
    return 1;
  }
  [[nodiscard]] size_t max_arguments() const override {
    return 1;
  }

  int run_inner(vector<string>&& arguments) override {
    const auto next_revision = repo().next_revision();
    const auto current_revision = repo().current_revision();
    const auto merge_revision = repo().merge_revision();
    const auto commit =
        Commit(next_revision, current_revision, merge_revision, chrono::system_clock::now(), arguments[0]);

    cout << "Creating commit " << commit.id() << " '" << commit.message() << "'" << endl;

    repo().create_commit(commit, current_revision);

    return 0;
  }
};

} // namespace lit
