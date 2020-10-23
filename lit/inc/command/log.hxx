#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <numeric>

#include "command.hxx"
#include "repository.hxx"

namespace lit {

using namespace std;

class LogCommand: public Command {
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

    vector<shared_ptr<Commit>> commits(revisions, nullptr);
    vector<vector<size_t>> children(revisions, vector<size_t>());

    for (size_t i = 0; i <= latest_revision_n; i++) {
      const auto commit = repo().commit(Revision(i));

      const auto parent_a = commit.parent_a();
      const auto parent_b = commit.parent_b();

      commits[i] = shared_ptr<Commit>(new Commit(commit));

      if (parent_a) {
        children[parent_a->number()].push_back(i);
      }

      if (parent_b) {
        children[parent_b->number()].push_back(i);
      }
    }

    vector<vector<size_t>> branches(revisions, vector<size_t>());
    size_t max_branch_size = 0;

    for (size_t i = 0; i <= latest_revision_n; i++) {
      const auto commit = commits[i];

      const auto parent_a = commit->parent_a();

      if (i == 0) {
        branches[i].push_back(i);
      } else {
        bool marked_self = false;

        for (size_t p = 0; p < branches[i - 1].size(); p++) {
          const auto parent = branches[i - 1][p];

          if (parent_a && parent == parent_a->number() && !marked_self) {
            branches[i].push_back(i);
            marked_self = true;
          } else {
            branches[i].push_back(parent);
          }
        }
      }

      for (size_t c = 0; (c + 1) < children[i].size(); c++) {
        const auto child = children[i][c];

        const auto right_parent = commits[child]->parent_b();
        if (right_parent && i == right_parent->number()) {
          continue;
        }

        branches[i].push_back(i);
      }

      max_branch_size = max(max_branch_size, branches[i].size());
    }

    // cout << "Branches: " << endl;
    //
    // for (auto i = latest_revision_n; i <= latest_revision_n; i--) {
    //   cout << i << ": ";
    //
    //   for (auto& b: branches[i]) {
    //     cout << b << " ";
    //   }
    //
    //   cout << endl;
    // }

    for (size_t i = latest_revision_n; i <= latest_revision_n; i--) {
      const auto commit = commits[i];

      const auto parent_b = commit->parent_b();

      bool printed_self = false;
      bool printed_right_parent = false;

      for (size_t b = 0; b < branches[i].size(); b++) {
        const auto r = branches[i][b];

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
          const bool is_right_parent = parent_b && parent_b->number() == r;

          if (is_right_parent) {
            if (printed_right_parent) {
              cout << " │";
            } else {
              size_t right_parent_children = children[parent_b->number()].size();

              cout << "─";
              if (right_parent_children == 1 || i == latest_revision_n) {
                cout << "┐";
              } else {
                cout << "┤";
              }

              printed_right_parent = true;
            }
          } else {
            bool i_crosses_current = false;

            if (b > 0) {
              if ((b + 1) < branches[i].size()) {
                if (!printed_right_parent) {
                  const auto left = branches[i][b - 1];
                  const auto right = branches[i][b + 1];

                  if (left == i && left == right) {
                    i_crosses_current = true;
                  } else if (parent_b) {
                    for (auto b2 = b; b2 < branches[i].size(); b2++) {
                      if (branches[i][b2] == parent_b->number()) {
                        i_crosses_current = true;
                        break;
                      }
                    }
                  }
                }
              }

              if (i_crosses_current) {
                cout << "─";
              } else {
                cout << " ";
              }
            }

            const size_t max_child = accumulate(children[r].begin(), children[r].end(), 0,
                                              [](size_t acc, size_t child) { return max(acc, child); });

            if (i <= max_child) {
              cout << "│";
            } else if (i_crosses_current) {
              cout << "─";
            } else {
              cout << " ";
            }
          }
        }
      }

      for (size_t b = branches[i].size(); b < max_branch_size; b++) {
        cout << "  ";
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
