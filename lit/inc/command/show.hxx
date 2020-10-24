#pragma once

#include <fstream>

#ifndef LIT_COMMAND_HXX
#include "command.hxx"
#endif

#include "repository.hxx"
#include "revision.hxx"

namespace lit {

using namespace std;

class ShowCommand: public Command {
  public:
  [[nodiscard]] string description() const override {
    return "Inspect the given commit.";
  }
  [[nodiscard]] size_t max_arguments() const override {
    return 1;
  }

  int run_inner(vector<string>&& arguments) override {
    optional<Revision> revision;

    if (arguments.size() == 1) {
      revision = optional(Revision(move(arguments[0])));
    } else {
      revision = repo().current_revision();
    }

    const auto commit = repo().commit(revision.value());
    commit.pretty_print(cout);

    cout << endl;

    ifstream patch(repo().revisions_dir() / revision.value().patch_filename());
    cout << patch.rdbuf();

    return 0;
  }
};

} // namespace lit
