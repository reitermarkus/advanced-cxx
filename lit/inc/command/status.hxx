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
  string description() const override {
    return "Show the working tree status.";
  }

  int run_inner(vector<string>&& arguments) override {
    for (const auto& entry: repo().status()) {
      const auto [path, status] = entry;
      cout << to_char(status) << "  " << path << endl;
    }

    return 0;
  }
};

}
