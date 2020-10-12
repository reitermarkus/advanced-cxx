#pragma once

#include <cassert>
#include <filesystem>

#include "sub_process.hxx"

using namespace std;
namespace fs = std::filesystem;

class Diff {
  bool is_changed;
  string patch;

  public:
  Diff(fs::path a, fs::path b, string label_a, string label_b) {
    SubProcess diff("diff");
    diff.arg("-u");
    diff.arg(string(a)).arg(string(b));
    diff.arg("--label").arg(label_a);
    diff.arg("--label").arg(label_b);

    auto [output, status] = diff.output();

    assert(status == 0 || status == 1);

    this->is_changed = status == 1;
    this->patch = output;
  }
  Diff(fs::path a, fs::path b): Diff(a, b, string(a), string(b)) {}

  bool is_empty() {
    return this->is_changed;
  }

  string& output() {
    return this->patch;
  }
};
