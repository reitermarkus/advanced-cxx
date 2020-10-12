#pragma once

#include "command.hxx"

using namespace std;

namespace command {

class Log: public Command {
  public:
  string name() const override {
    return "log";
  }
  string description() const override {
    return "Display a graph of all commits.";
  }

  int run(vector<string>& arguments) override {
    throw;
  }
};

}
