#pragma once

#include <algorithm>
#include <functional>

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

  int run_inner(vector<string>& arguments) override {
    auto latest_revision = repo().latest_revision().value();
    auto current_revision = repo().current_revision().value();

    auto latest_revision_n = latest_revision.number();
    auto revisions = latest_revision_n + 1;
    vector<vector<long long>> children(revisions, vector<long long>());
    vector<decltype(latest_revision_n)> unfinished(revisions, 0);

    size_t indentation = 0;
    size_t max_indentation = 0;

    bool previous_was_merge_commit = false;

    for (ssize_t i = latest_revision_n; i >= 0; i--) {
      auto commit = repo().commit(Revision(i));

      auto parent_a = commit.parent_a();
      auto parent_b = commit.parent_b();

      if (parent_a) {
        children[parent_a.value().number()].push_back(i);
      }

      if (parent_b) {
        children[parent_b.value().number()].push_back(i);
      }

      auto is_root_commit = !bool(parent_a);
      auto is_merge_commit = bool(parent_b);

      for (auto& child: children[i]) {
        unfinished[child] -= 1;

        if (unfinished[child] == 0) {
          indentation -= 1;
        }
      }

      auto indentation_left = max_indentation;

      for (size_t j = 0; j < indentation; j++) {
        cout << "│ ";
        indentation_left -= 1;
      }

      cout << "◯";

      if (is_root_commit) {
        if (indentation_left) {
          cout << "─┘";
        }
      } else if (is_merge_commit) {
        if (indentation_left) {
          cout << "─┤";
        } else {
          cout << "─┐";
        }

      } else {
        for (size_t j = 0; j < indentation_left; j++) {
          cout << " │";
        }
      }

      cout << " ";
      if (i == current_revision.number()) {
        cout << "← ";
      } else {
        cout << "  ";
      }

      cout << commit.id() << " \"" << commit.message() << "\"" << endl;

      if (!previous_was_merge_commit) {
        unfinished[i] += is_merge_commit ? 2 : 1;
        indentation += 1;
        max_indentation = max(max_indentation, indentation);
      }

      previous_was_merge_commit = is_merge_commit;
    }

    return 0;
  }
};

}
