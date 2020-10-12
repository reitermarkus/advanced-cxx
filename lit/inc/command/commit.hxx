#pragma once

#include "command.hxx"

using namespace std;

namespace command {

class Commit: public Command {
  public:
  string name() const override {
    return "commit";
  }
  string description() const override {
    return "Record changes to the repository.";
  }

  int run(vector<string>& arguments) override {
    throw;
  }
};

}
