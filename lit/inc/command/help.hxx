#pragma once

#include "command.hxx"

using namespace std;

namespace command {

class Help: public Command {
  public:
  string name() const override {
    return "help";
  }
  string description() const override {
    return "Display help information about Lit.";
  }

  int run_inner(vector<string>&& arguments) override {
    Command::print_usage(cout);

    return 0;
  }
};

}
