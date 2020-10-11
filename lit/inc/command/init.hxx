#pragma once

#include <filesystem>
#include <iostream>

#include "command.hxx"
#include "repository.hxx"

using namespace std;
namespace fs = std::filesystem;

namespace command {

class Init: public Command {
  public:
  string name() const override {
    return "init";
  }
  string description() const override {
    return "Initialize a repository.";
  }

  int run(vector<string>& arguments) override {
    if (!arguments.empty()) {
      cerr << "This command does not support arguments." << endl;
      return 1;
    }

    auto repo = Repository();
    auto lit_dir = string(repo.lit_dir());
    if (repo.is_initialized()) {
      cout << "Re-initialized existing Lit repository in " << lit_dir << endl;
    } else {
      repo.initialize();
      cout << "Initialized empty Lit repository in " << lit_dir << endl;
    }

    return 0;
  }
};

}