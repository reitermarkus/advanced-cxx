#pragma once

#include "command.hxx"

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
    throw;
  }
};

}
