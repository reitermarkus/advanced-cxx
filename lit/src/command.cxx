#include "command.hxx"
#include "command/help.hxx"

vector<unique_ptr<Command>> Command::list() {
  vector<unique_ptr<Command>> commands;

  commands.emplace_back(unique_ptr<Command>(new command::Help()));

  return commands;
}
