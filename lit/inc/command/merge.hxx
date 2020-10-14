#pragma once

#include "command.hxx"

using namespace std;

namespace command {

class Merge: public Command {
  public:
  string name() const override {
    return "merge";
  }
  string description() const override {
    return "Initiate a merge with the currently checked out commit and the specified commit.";
  }

  int run(vector<string>& arguments) override {
    if (arguments.size() != 1) {
      cerr << "This command requires exactly one argument." << endl;
      return 1;
    }

    auto repo = Repository();

    auto status = repo.status();
    if (!status.empty()) {
      cerr << "Cannot merge with uncommitted files." << endl;
      return 1;
    }

    auto current_revision = repo.current_revision().value();
    auto merge_revision = Revision(arguments[0]);

    cout << "Merging " << merge_revision.id() << " into " << current_revision.id() << endl;

    auto revision_a = current_revision;
    auto revision_b = merge_revision;

    while (revision_a != revision_b) {
      if (revision_a > revision_b) {
        revision_a = repo.commit(revision_a).parent_a().value();
      } else if (revision_b > revision_a) {
        revision_b = repo.commit(revision_b).parent_a().value();
      } else {
        break;
      }
    }
    auto base = current_revision;

    cout << "Merge base: " << base.id() << endl;

    // return 0;
    return 1;
  }
};

}
