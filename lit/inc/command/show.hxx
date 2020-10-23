#pragma once

#include <fstream>

#include "command.hxx"
#include "repository.hxx"
#include "revision.hxx"

namespace lit {
namespace command {

using namespace std;

class Show: public Command {
  public:
  string name() const override {
    return "show";
  }
  string description() const override {
    return "Inspect the given commit.";
  }
  size_t max_arguments() const override {
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

}
}
