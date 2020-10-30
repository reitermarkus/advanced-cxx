#pragma once

#include <deque>

#ifndef LIT_COMMAND_HXX
#include "command.hxx"
#endif

#include "diff.hxx"
#include "dir_diff.hxx"
#include "fs.hxx"
#include "patch.hxx"
#include "repository.hxx"
#include "revision.hxx"

namespace lit {

using namespace std;

class CheckoutCommand: public Command {
  public:
  [[nodiscard]] string description() const override {
    return "Reset the repository to the given commit's state.";
  }
  [[nodiscard]] size_t max_arguments() const override {
    return 1;
  }

  int run_inner(vector<string>&& arguments) override {
    const auto target_revision = arguments.empty() ? *repo().current_revision() : Revision(arguments[0]);

    repo().checkout(target_revision);

    return 0;
  }
};

} // namespace lit
