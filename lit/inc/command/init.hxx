#pragma once

#include <filesystem>
#include <iostream>

#include "command.hxx"
#include "fs.hxx"
#include "repository.hxx"

using namespace std;

namespace command {

class Init: public Command {
  public:
  string name() const override {
    return "init";
  }
  string description() const override {
    return "Create an empty Lit repository or reinitialize an existing one";
  }

  int run_inner(vector<string>&& arguments) override {
    const auto lit_dir = string(repo().lit_dir());
    if (repo().is_initialized()) {
      cout << "Reinitialized existing Lit repository in " << lit_dir << endl;
    } else {
      repo().initialize();
      cout << "Initialized empty Lit repository in " << lit_dir << endl;
    }

    return 0;
  }
};

}
