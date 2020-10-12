#pragma once

#include "command.hxx"

using namespace std;

namespace command {

class Merge: public Command {
  public:
  string name() const override {
    return "merge";
  }
  string description() const override {
    return "Initiate a merge with the currently checked out commit and the specified commit.";
  }

  int run(vector<string>& arguments) override {
    throw;
  }
};

}
