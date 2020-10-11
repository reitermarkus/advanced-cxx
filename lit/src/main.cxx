#include <iostream>
#include <unordered_map>
#include <vector>

#include "command.hxx"
#include "command/help.hxx"

using namespace std;

int main(int argc, char* argv[]) {
  vector<string> arguments(argv + 1, argv + argc);

  auto commands = Command::list();

  if (arguments.empty()) {
    cerr << "No subcommand given." << endl;
    return 1;
  }

  auto command_name = arguments.at(0);

  for (auto& command: commands) {
    if (command->name() == command_name) {
      return command->run(arguments);
    }
  }

  cerr << "Unknown subcommand: " << command_name << endl;
  return 1;
}
