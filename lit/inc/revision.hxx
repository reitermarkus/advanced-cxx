#pragma once

#include <string>

using namespace std;

class Revision {
  unsigned long number_;

  public:
  Revision(string id) {
    assert(id[0] == 'r');
    id.erase(0, 1);
    this->number_ = stoul(id);
  }
  Revision(unsigned long number) {
    this->number_ = number;
  }

  unsigned long number() {
    return this->number_;
  }

  string id() {
    return "r" + to_string(number());
  }

  string meta_filename() {
    return id() + ".txt";
  }

  string patch_filename() {
    return id() + ".patch";
  }
};
