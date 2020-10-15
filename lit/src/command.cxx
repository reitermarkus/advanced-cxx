#include <numeric>

#include "command.hxx"
#include "command/checkout.hxx"
#include "command/commit.hxx"
#include "command/help.hxx"
#include "command/init.hxx"
#include "command/log.hxx"
#include "command/merge.hxx"
#include "command/show.hxx"
#include "command/status.hxx"

vector<unique_ptr<Command>> Command::list() {
  vector<unique_ptr<Command>> commands;

  commands.emplace_back(unique_ptr<Command>(new command::Checkout()));
  commands.emplace_back(unique_ptr<Command>(new command::Commit()));
  commands.emplace_back(unique_ptr<Command>(new command::Help()));
  commands.emplace_back(unique_ptr<Command>(new command::Init()));
  commands.emplace_back(unique_ptr<Command>(new command::Log()));
  commands.emplace_back(unique_ptr<Command>(new command::Merge()));
  commands.emplace_back(unique_ptr<Command>(new command::Show()));
  commands.emplace_back(unique_ptr<Command>(new command::Status()));

  return commands;
}

void Command::print_usage(ostream& stream) {
  auto commands = Command::list();

  auto max_name_length = [](size_t acc, unique_ptr<Command>& command) { return max(acc, command->name().length()); };
  auto max_length = accumulate(commands.begin(), commands.end(), 0, max_name_length);

  stream << "Usage: lit <command> [<args>]" << endl;
  stream << endl;
  stream << "Commands:" << endl;
  for (auto& command: commands) {
    auto name = command->name();
    auto description = command->description();
    stream << "  " << name << string(max_length - name.length(), ' ') << "  " << description << endl;
  }
}
