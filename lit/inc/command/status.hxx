#pragma once

#include <filesystem>
#include <functional>
#include <iostream>
#include <numeric>

#ifndef LIT_COMMAND_HXX
#include "command.hxx"
#endif

#include "fs.hxx"
#include "repository.hxx"

namespace lit {

using namespace std;

class StatusCommand: public Command {
  public:
  string name() const override {
    return "status";
  }
  string description() const override {
    return "Show the working tree status.";
  }

  int run_inner(vector<string>&& arguments) override {
    const auto file_statuses = repo().status();

    for (const auto& entry: file_statuses) {
      const auto [path, status] = entry;

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
