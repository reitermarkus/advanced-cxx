#pragma once

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "repository.hxx"

using namespace std;

class Command {
  size_t min_arguments = 0;
  size_t max_arguments = 0;

  optional<Repository> repo_;

  optional<string> check_arguments(vector<string>& arguments) {
    if (min_arguments == max_arguments) {
      if (min_arguments != arguments.size()) {
        if (min_arguments == 0) {
          return optional("This command does not support arguments.");
        } else {
          return optional("This command only supports exactly " + to_string(min_arguments) + " arguments.");
        }
      }
    } else if (arguments.size() < min_arguments) {
      if (min_arguments == 1) {
        return optional("This command supports at least one argument.");
      } else {
        return optional("This command supports at least " + to_string(min_arguments) + " arguments.");
      }
    } else if (arguments.size() > max_arguments) {
      if (max_arguments == 1) {
        return optional("This command supports at most one argument.");
      } else {
        return optional("This command supports at most " + to_string(max_arguments) + " arguments.");
      }
    }

    return nullopt;
  }

  public:
  Command(size_t min_args = 0, size_t max_args = 0): min_arguments(min_args), max_arguments(max_args) {}
  virtual ~Command() {}

  virtual string name() const = 0;
  virtual string description() const = 0;
  virtual int run_inner(vector<string>& arguments) = 0;
  int run(vector<string>& arguments) {
    auto argument_error = check_arguments(arguments);
    if (argument_error) {
      cerr << *argument_error << endl;
      cerr << endl;
      print_usage(cerr);
      return 1;
    }

    return run_inner(arguments);
  }

  static vector<unique_ptr<Command>> list();

  static void print_usage(ostream& stream);

  Repository repo() {
    if (!repo_) {
      repo_ = optional(Repository());
    }

    return *repo_;
  }
};
