#pragma once

#include <deque>

#include "command.hxx"
#include "patch.hxx"
#include "repository.hxx"
#include "revision.hxx"

using namespace std;

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

    if (arguments.size() != 1) {
      // TODO: Reset repo.
      return 1;
    }

    auto target_revision = Revision(arguments[0]);
    auto current_revision = repo.current_revision().value();

    if (current_revision == target_revision) {
      return 0;
    }

    deque<Revision> patches;

    bool backwards = target_revision < current_revision;

    auto start_revision = backwards ? current_revision : target_revision;
    auto end_revision = backwards ? target_revision : current_revision;

    auto commit = repo.commit(start_revision);
    while (commit.revision() != end_revision) {
      if (backwards) {
        patches.push_back(commit.revision());
      } else {
        patches.push_front(commit.revision());
      }

      auto parent = commit.parent_a();

      if (parent) {
        commit = repo.commit(parent.value());
      } else {
        break;
      }
    }

    for (auto& revision: patches) {
      auto patch_file_path = repo.revisions_dir() / revision.patch_filename();

      Patch patch(repo.dir(), patch_file_path);

      if (backwards) {
        cout << "Reverting " << revision.id() << endl;
        patch.revert();
      } else {
        cout << "Applying " << revision.id() << endl;
        patch.apply();
      }
    }

    repo.write_revision(target_revision);

    return 0;
  }
};

}
