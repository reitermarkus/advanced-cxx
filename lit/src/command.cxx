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

  commands.emplace_back(new CheckoutCommand());
  commands.emplace_back(new CommitCommand());
  commands.emplace_back(new HelpCommand());
  commands.emplace_back(new InitCommand());
  commands.emplace_back(new LogCommand());
  commands.emplace_back(new MergeCommand());
  commands.emplace_back(new ShowCommand());
  commands.emplace_back(new StatusCommand());

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
