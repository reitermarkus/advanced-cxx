#pragma once

#include "command.hxx"

using namespace std;

namespace command {

class Show: public Command {
  public:
  string name() const override {
    return "show";
  }
  string description() const override {
    return "Inspect the given commit.";
  }

  int run(vector<string>& arguments) override {
    if (arguments.size() > 1) {
      cerr << "This command supports at most one argument." << endl;
      return 1;
    }

    auto repo = Repository();

    cout << repo.revision().value().id() << endl;

    return 0;
  }
};

}
