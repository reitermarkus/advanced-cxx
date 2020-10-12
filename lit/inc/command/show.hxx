#pragma once

#include "command.hxx"

using namespace std;

namespace command {

class Show: public Command {
  public:
  string name() const override {
    return "show";
  }
  string description() const override {
    return "Inspect the given commit.";
  }

  int run(vector<string>& arguments) override {
    throw;
  }
};

}
