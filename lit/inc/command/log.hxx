#pragma once

#include "command.hxx"
#include "repository.hxx"

using namespace std;

namespace command {

class Log: public Command {
  public:
  string name() const override {
    return "log";
  }
  string description() const override {
    return "Display a graph of all commits.";
  }

  int run(vector<string>& arguments) override {
    if (!arguments.empty()) {
      cerr << "This command does not support arguments." << endl;
      return 1;
    }

    auto repo = Repository();

    auto latest_revision = repo.latest_revision().value();
    auto current_revision = repo.current_revision().value();

    auto parents = 0;

    for (auto i = latest_revision.number(); i > 0; i--) {
      auto commit = repo.commit(Revision(i));

      auto parent_a = commit.parent_a();

      for (auto p = 0; p < parents; p++) {
        cout << "| ";
      }

      if (parent_a && parent_a.value().number() != i - 1) {
        parents += 1;
      }

      cout << "o";

      if (commit.parent_b()) {
        cout << "─┐";
        parents += 1;
      } else {
        cout << "  ";
      }

      cout << " ";
      if (i == current_revision.number()) {
        cout << "← ";
      } else {
        cout << "  ";
      }

      cout << commit.id() << " \"" << commit.message() << "\"" << endl;
    }

    return 0;
  }
};

}
