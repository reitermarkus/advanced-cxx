#pragma once

#include <algorithm>
#include <functional>
#include <memory>

#include "command.hxx"
#include "repository.hxx"

namespace lit {
namespace command {

using namespace std;

class Log: public Command {
  public:
  string name() const override {
    return "log";
  }
  string description() const override {
    return "Display a graph of all commits.";
  }

  int run_inner(vector<string>&& arguments) override {
    const auto latest_revision = repo().latest_revision().value();
    const auto current_revision = repo().current_revision().value();

    const auto latest_revision_n = latest_revision.number();
    const auto revisions = latest_revision_n + 1;

    vector<shared_ptr<::lit::Commit>> commits(revisions, nullptr);
    vector<vector<shared_ptr<::lit::Commit>>> children(revisions, vector<shared_ptr<::lit::Commit>>());

    for (auto i = 0; i <= latest_revision_n; i++) {
      const auto commit = repo().commit(Revision(i));

      const auto parent_a = commit.parent_a();
      const auto parent_b = commit.parent_b();

      commits[i] = shared_ptr<::lit::Commit>(new ::lit::Commit(commit));

      if (parent_a) {
        children[parent_a->number()].push_back(commits[i]);
      }

      if (parent_b) {
        children[parent_b->number()].push_back(commits[i]);
      }
    }

    vector<vector<size_t>> branches(revisions, vector<size_t>());

    for (auto i = 0; i <= latest_revision_n; i++) {
      const auto commit = commits[i];

      const auto parent_a = commit->parent_a();
      const auto parent_b = commit->parent_b();

      auto current_branch = 0;

      if (i > 0) {
        while (current_branch < branches[i - 1].size()) {
          const auto parent = branches[i - 1][current_branch];

          if ((parent_a && parent == parent_a->number()) || (parent_b && parent == parent_b->number())) {
            cout << i << ": break at " << current_branch << " = " << parent << endl;
            current_branch++;
            break;
          }

          branches[i].push_back(parent);
          current_branch++;
        }
      }

      branches[i].push_back(i);

      const auto children_count = children[i].size();
      for (auto c = 0; (c + 1) < children_count; c++) {
        auto child = children[i][c];

        auto right_parent = child->parent_b();
        if (right_parent && i == right_parent->number()) {
          cout << commit->id() << " is right parent of " << child->id() << endl;
          continue;
        }

        branches[i].push_back(i);
      }

      if (i > 0) {
        for (auto b = branches[i].size(); b < branches[i - 1].size(); b++) {
          const auto parent = branches[i - 1][b];

          branches[i].push_back(parent);
        }
      }
    }

    for (auto i = latest_revision_n; i <= latest_revision_n; i--) {
      auto commit = commits[i];

      auto parent_b = commit->parent_b();

      bool printed_self = false;

      for (auto b = 0; b < branches[i].size(); b++) {
        auto r = branches[i][b];

        if (r == i) {
          if (printed_self) {
            cout << (b == branches[i].size() - 1 ? "─┘" : "─┴");
          } else {
            if (b > 0) {
              cout << " ";
            }

            cout << "◯";
            printed_self = true;
          }
        } else {
          bool is_right_parent = parent_b && parent_b->number() == r;

          if (is_right_parent) {
            cout << "─";

            size_t right_parent_children = children[parent_b->number()].size();

            switch (right_parent_children) {
              case 1: {
                cout << "┐";
                break;
              }
              case 2: {
                cout << "┤";
                break;
              }
              default: {
                cout << "┼";
                break;
              }
            }

          } else {
            if (b > 0) {
              cout << " ";
            }

            bool has_children = children[r].size() > 0;
            if (has_children) {
              cout << "│";
            } else {
              cout << " ";
            }
          }
        }
      }

      cout << " ";

      if (i == current_revision.number()) {
        cout << "← ";
      } else {
        cout << "  ";
      }

      cout << commit->id() << ": " << commit->message() << endl;
    }

    return 0;
  }
};

}
}
