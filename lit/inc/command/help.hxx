#pragma once

#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

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

  int run(vector<string>& arguments) override {
    if (!arguments.empty()) {
      cerr << "This command does not support arguments." << endl;
      return 1;
    }

    Command::print_usage(cout);

    return 0;
  }
};

}
