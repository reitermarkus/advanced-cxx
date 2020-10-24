#ifndef LIT_COMMAND_HXX
#define LIT_COMMAND_HXX

#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>

#include "repository.hxx"

namespace lit {

using namespace std;

class Command {
  optional<Repository> repo_;

  [[nodiscard]] optional<string> check_arguments(const vector<string>& arguments) const noexcept {
    if (min_arguments() == max_arguments()) {
      if (min_arguments() != arguments.size()) {
        if (min_arguments() == 0) {
          return optional("This command does not support arguments.");
        }

        return optional("This command only supports exactly " + to_string(min_arguments()) + " arguments.");
      }
    } else if (arguments.size() < min_arguments()) {
      if (min_arguments() == 1) {
        return optional("This command supports at least one argument.");
      }

      return optional("This command supports at least " + to_string(min_arguments()) + " arguments.");
    } else if (arguments.size() > max_arguments()) {
      if (max_arguments() == 1) {
        return optional("This command supports at most one argument.");
      }

      return optional("This command supports at most " + to_string(max_arguments()) + " arguments.");
    }

    return nullopt;
  }

  public:
  Command() noexcept = default;
  Command(Command&&) = default;
  Command(const Command&) = default;
  virtual ~Command() = default;
  Command& operator=(const Command&) = default;
  Command& operator=(Command&&) = default;

  [[nodiscard]] virtual string description() const = 0;

  [[nodiscard]] virtual size_t min_arguments() const {
    return 0;
  };

  [[nodiscard]] virtual size_t max_arguments() const {
    return 0;
  };

  virtual int run_inner(vector<string>&& arguments) = 0;

  int run(vector<string>&& arguments) {
    const auto argument_error = check_arguments(arguments);
    if (argument_error) {
      cerr << *argument_error << endl;
      cerr << endl;
      print_usage(cerr);
      return 1;
    }

    return run_inner(move(arguments));
  }

  static map<string, unique_ptr<Command>> list();

  static void print_usage(ostream& stream) {
    auto commands = Command::list();

    const auto max_name_length = [](size_t acc, auto& pair) { return max(acc, pair.first.length()); };
    const auto max_length = accumulate(commands.begin(), commands.end(), 0, max_name_length);

    stream << "Usage: lit <command> [<args>]" << endl;
    stream << endl;
    stream << "Commands:" << endl;
    for (const auto& pair: commands) {
      const auto& [name, command] = pair;
      const auto description = command->description();
      stream << "  " << name << string(max_length - name.length(), ' ') << "  " << description << endl;
    }
  }

  Repository repo() {
    if (!repo_) {
      repo_ = optional(Repository());
    }

    return *repo_;
  }
};

} // namespace lit

#include "command/checkout.hxx"
#include "command/commit.hxx"
#include "command/help.hxx"
#include "command/init.hxx"
#include "command/log.hxx"
#include "command/merge.hxx"
#include "command/show.hxx"
#include "command/status.hxx"

namespace lit {

map<string, unique_ptr<Command>> Command::list() {
  map<string, unique_ptr<Command>> commands;

  commands.emplace("checkout", new CheckoutCommand());
  commands.emplace("commit", new CommitCommand());
  commands.emplace("help", new HelpCommand());
  commands.emplace("init", new InitCommand());
  commands.emplace("log", new LogCommand());
  commands.emplace("merge", new MergeCommand());
  commands.emplace("show", new ShowCommand());
  commands.emplace("status", new StatusCommand());

  return commands;
}

} // namespace lit

#endif
