#include <iostream>
#include <unordered_map>
#include <vector>

#include "command.hxx"
#include "command/help.hxx"

using namespace std;

int main(int argc, char* argv[]) {
  if (argc == 1) {
    cerr << "No subcommand given." << endl;
    cerr << endl;
    Command::print_usage(cerr);
    return 1;
  }

  const auto command_name = string(argv[1]);

  const auto commands = Command::list();

  for (const auto& command: commands) {
    if (command->name() == command_name) {
      vector<string> arguments(argv + 2, argv + argc);
      return command->run(move(arguments));
    }
  }

  cerr << "Unknown subcommand: " << command_name << endl;
  cerr << endl;
  Command::print_usage(cerr);
  return 1;
}
