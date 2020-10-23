#pragma once

#include <cassert>
#include <filesystem>

#include "fs.hxx"
#include "sub_process.hxx"

namespace lit {

using namespace std;

class Diff {
  bool is_changed_;
  string patch;

  public:
  Diff(const fs::path& a, const fs::path& b, const string& label_a, const string& label_b) {
    SubProcess diff("diff");
    diff.arg("-u");
    diff.arg("-N");
    diff.arg(string(a)).arg(string(b));
    diff.arg("--label").arg(label_a);
    diff.arg("--label").arg(label_b);

    const auto [output, status] = diff.output();

    assert(status == 0 || status == 1);

    this->is_changed_ = status == 1;
    this->patch = output;
  }
  Diff(const fs::path& a, const fs::path& b): Diff(a, b, string(a), string(b)) {}

  bool is_changed() const {
    return this->is_changed_;
  }

  const string& output() const {
    return this->patch;
  }
};

}
