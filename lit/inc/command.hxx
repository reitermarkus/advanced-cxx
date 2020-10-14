#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Command {
  public:
  Command() {}
  virtual ~Command() {}

  virtual string name() const {
    throw;
  };
  virtual string description() const {
    throw;
  };
  virtual int run(vector<string>& arguments) = 0;

  static vector<unique_ptr<Command>> list();

  static void print_usage(ostream& stream);
};
