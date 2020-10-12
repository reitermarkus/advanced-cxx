#pragma once

#include "command.hxx"

using namespace std;

namespace command {

class Checkout: public Command {
  public:
  string name() const override {
    return "checkout";
  }
  string description() const override {
    return "Reset the repository to the given commit's state.";
  }

  int run(vector<string>& arguments) override {
    throw;
  }
};

}
