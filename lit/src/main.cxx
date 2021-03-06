#include <exception>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "command.hxx"

using namespace std;
using namespace lit;

int main(int argc, char* argv[]) {
  try {
    if (argc == 1) {
      cerr << "No subcommand given." << endl;
      cerr << endl;
      Command::print_usage(cerr);
      return 1;
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    const auto command_name = string(argv[1]);

    auto commands = Command::list();

    if (commands.find(command_name) == commands.end()) {
      cerr << "Unknown subcommand: " << command_name << endl;
      cerr << endl;
      Command::print_usage(cerr);
      return 1;
    }

    vector<string> arguments(argv + 2, argv + argc);
    return commands[command_name]->run(move(arguments));

  } catch (exception& e) {
    cerr << "Exception occured: " << e.what() << endl;
    return 1;
  }
}
