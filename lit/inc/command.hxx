#ifndef LIT_COMMAND_HXX
#define LIT_COMMAND_HXX

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "repository.hxx"

namespace lit {

using namespace std;

class Command {
  optional<Repository> repo_;

  optional<string> check_arguments(const vector<string>& arguments) {
    if (min_arguments() == max_arguments()) {
      if (min_arguments() != arguments.size()) {
        if (min_arguments() == 0) {
          return optional("This command does not support arguments.");
        } else {
          return optional("This command only supports exactly " + to_string(min_arguments()) + " arguments.");
        }
      }
    } else if (arguments.size() < min_arguments()) {
      if (min_arguments() == 1) {
        return optional("This command supports at least one argument.");
      } else {
        return optional("This command supports at least " + to_string(min_arguments()) + " arguments.");
      }
    } else if (arguments.size() > max_arguments()) {
      if (max_arguments() == 1) {
        return optional("This command supports at most one argument.");
      } else {
        return optional("This command supports at most " + to_string(max_arguments()) + " arguments.");
      }
    }

    return nullopt;
  }

  public:
  Command() {}
  virtual ~Command() {}

  virtual string name() const = 0;
  virtual string description() const = 0;

  virtual size_t min_arguments() const {
    return 0;
  };

  virtual size_t max_arguments() const {
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

  static vector<unique_ptr<Command>> list();

  static void print_usage(ostream& stream) {
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

  Repository repo() {
    if (!repo_) {
      repo_ = optional(Repository());
    }

    return *repo_;
  }
};

}

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

}

#endif
