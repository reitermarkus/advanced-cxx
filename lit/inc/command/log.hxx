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

    auto revision = repo.latest_revision().value();
    auto current_revision = repo.current_revision().value();

    auto commit = repo.commit(revision);

    while (true) {
      cout << "o ";

      if (commit.revision().number() == current_revision.number()) {
        cout << "← ";
      } else {
        cout << "  ";
      }

      cout << commit.id() << " \"" << commit.message() << "\"" << endl;

      if (commit.parent_a()) {
        commit = repo.commit(commit.parent_a().value());
      } else {
        break;
      }
    }

    return 0;
  }
};

}
