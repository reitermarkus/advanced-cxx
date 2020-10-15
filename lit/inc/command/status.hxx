#pragma once

#include <filesystem>
#include <functional>
#include <iostream>
#include <numeric>

#include "command.hxx"
#include "fs.hxx"
#include "repository.hxx"

using namespace std;

namespace command {

class Status: public Command {
  public:
  string name() const override {
    return "status";
  }
  string description() const override {
    return "Show the working tree status.";
  }

  int run(vector<string>& arguments) override {
    if (!arguments.empty()) {
      cerr << "This command does not support arguments." << endl;
      return 1;
    }

    auto repo = Repository();

    auto file_statuses = repo.status();

    for (auto& entry: file_statuses) {
      auto [path, status] = entry;

      switch (status) {
        case Added: {
          cout << "A";
          break;
        }
        case Modified: {
          cout << "M";
          break;
        }
        case Deleted: {
          cout << "D";
          break;
        }
      }

      cout << "  " << path << endl;
    }

    return 0;
  }
};

}
