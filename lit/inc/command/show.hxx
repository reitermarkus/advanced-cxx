#pragma once

#include <fstream>

#include "command.hxx"
#include "repository.hxx"
#include "revision.hxx"

using namespace std;

namespace command {

class Show: public Command {
  public:
  Show(): Command(0, 1) {}
  string name() const override {
    return "show";
  }
  string description() const override {
    return "Inspect the given commit.";
  }

  int run_inner(vector<string>& arguments) override {
    optional<Revision> revision;

    if (arguments.size() == 1) {
      revision = optional(Revision(arguments[0]));
    } else {
      revision = repo().current_revision();
    }

    auto commit = repo().commit(revision.value());
    commit.pretty_print(cout);

    cout << endl;

    ifstream patch(repo().revisions_dir() / revision.value().patch_filename());
    cout << patch.rdbuf();

    return 0;
  }
};

}
