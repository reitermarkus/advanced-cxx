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
  string name() const override { return "help"; }

  string description() const override { return "Show help information."; }

  int run(vector<string>& arguments) override {
    auto commands = Command::list();

    auto max_name_length = [](size_t acc, unique_ptr<Command>& command) { return max(acc, command->name().length()); };
    auto max_length = accumulate(commands.begin(), commands.end(), 0, max_name_length);

    cout << "Usage: lit <command> [<args>]" << endl;
    cout << endl;
    cout << "Commands:" << endl;
    for (auto& command: commands) {
      auto name = command->name();
      auto description = command->description();
      cout << "  " << name << string(max_length - name.length(), ' ') << "  " << description << endl;
    }

    return 0;
  }
};

}
