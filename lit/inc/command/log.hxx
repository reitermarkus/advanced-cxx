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

    vector<vector<long long>> children(latest_revision.number() + 1, vector<long long>());

    for (long long i = latest_revision.number(); i >= 0; i--) {
      auto commit = repo.commit(Revision(i));

      auto parent_a = commit.parent_a();
      auto parent_b = commit.parent_b();

      if (parent_a) {
        children[parent_a.value().number()].push_back(i);
      }

      cout << "o";

      if (parent_b) {
        cout << "─┐";
        children[parent_b.value().number()].push_back(i);
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
