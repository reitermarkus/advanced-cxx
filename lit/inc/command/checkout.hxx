#pragma once

#include <deque>

#include "command.hxx"
#include "diff.hxx"
#include "dir_diff.hxx"
#include "fs.hxx"
#include "patch.hxx"
#include "repository.hxx"
#include "revision.hxx"

using namespace std;

namespace command {

class Checkout: public Command {
  public:
  Checkout(): Command(0, 1) {}
  string name() const override {
    return "checkout";
  }
  string description() const override {
    return "Reset the repository to the given commit's state.";
  }

  int run_inner(vector<string>& arguments) override {
    auto target_revision = arguments.empty() ? *repo().current_revision() : Revision(arguments[0]);
    repo().checkout(target_revision);

    return 0;
  }
};

}
