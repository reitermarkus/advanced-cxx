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

namespace lit {

vector<unique_ptr<Command>> Command::list() {
  vector<unique_ptr<Command>> commands;

  commands.emplace_back(new command::Checkout());
  commands.emplace_back(new command::Commit());
  commands.emplace_back(new command::Help());
  commands.emplace_back(new command::Init());
  commands.emplace_back(new command::Log());
  commands.emplace_back(new command::Merge());
  commands.emplace_back(new command::Show());
  commands.emplace_back(new command::Status());

  return commands;
}

void Command::print_usage(ostream& stream) {
  const auto commands = Command::list();

  const auto max_name_length = [](size_t acc, const unique_ptr<Command>& command) {
    return max(acc, command->name().length());
  };
  const auto max_length = accumulate(commands.begin(), commands.end(), 0, max_name_length);

  stream << "Usage: lit <command> [<args>]" << endl;
  stream << endl;
  stream << "Commands:" << endl;
  for (const auto& command: commands) {
    const auto name = command->name();
    const auto description = command->description();
    stream << "  " << name << string(max_length - name.length(), ' ') << "  " << description << endl;
  }
}

}
