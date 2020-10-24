#pragma once

#include <cassert>
#include <iostream>
#include <string>

namespace lit {

using namespace std;

class Revision {
  unsigned long number_;

  public:
  Revision(string&& id) {
    if (id[0] != 'r') {
      cerr << "Invalid revision ID: '" << id << "'" << endl;
      abort();
    }

    id.erase(0, 1);
    this->number_ = stoul(id);
  }
  Revision(unsigned long number) {
    this->number_ = number;
  }

  unsigned long number() const {
    return this->number_;
  }

  string id() const {
    return "r" + to_string(this->number_);
  }

  string meta_filename() const {
    return id() + ".txt";
  }

  string patch_filename() const {
    return id() + ".patch";
  }

  bool operator==(const Revision& other) const {
    return this->number_ == other.number_;
  }

  bool operator>(const Revision& other) const {
    return this->number_ > other.number_;
  }

  bool operator<(const Revision& other) const {
    return this->number_ < other.number_;
  }
};

}
