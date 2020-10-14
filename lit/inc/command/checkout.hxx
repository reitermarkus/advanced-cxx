#pragma once

#include <deque>

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

class Checkout: public Command {
  public:
  string name() const override {
    return "checkout";
  }
  string description() const override {
    return "Reset the repository to the given commit's state.";
  }

  int run(vector<string>& arguments) override {
    if (arguments.size() > 1) {
      cerr << "This command supports at most one argument." << endl;
      return 1;
    }

    auto repo = Repository();

    auto target_revision = arguments.size() == 1 ? Revision(arguments[0]) : repo.current_revision().value();
    repo.checkout(target_revision);

    return 0;
  }
};

}
