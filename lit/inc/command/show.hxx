#pragma once

#include <fstream>

#include "command.hxx"
#include "repository.hxx"
#include "revision.hxx"

using namespace std;

namespace command {

class Show: public Command {
  public:
  string name() const override {
    return "show";
  }
  string description() const override {
    return "Inspect the given commit.";
  }

  int run(vector<string>& arguments) override {
    if (arguments.size() > 1) {
      cerr << "This command supports at most one argument." << endl;
      return 1;
    }

    auto repo = Repository();

    optional<Revision> revision;

    if (arguments.size() == 1) {
      revision = optional(Revision(arguments[0]));
    } else {
      revision = repo.current_revision();
    }

    ifstream meta(repo.revisions_dir() / revision.value().meta_filename());
    auto commit = ::Commit::deserialize(meta);
    commit.pretty_print(cout);

    cout << endl;

    ifstream patch(repo.revisions_dir() / revision.value().patch_filename());
    cout << patch.rdbuf();

    return 0;
  }
};

}
