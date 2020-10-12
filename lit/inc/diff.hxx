#include <cassert>
#include <filesystem>

#include "sub_process.hxx"

using namespace std;
namespace fs = std::filesystem;

class Diff {
  fs::path path_a;
  fs::path path_b;

  string output;

  public:
  Diff(fs::path a, fs::path b) {
    this->path_a = a;
    this->path_b = b;

    SubProcess diff("diff");
    diff.arg("-u");
    diff.arg(string(this->path_a)).arg(string(this->path_b));

    auto [output, status] = diff.output();
    assert(status == 0);

    this->output = output;
  }

  bool is_empty() {
    return this->output == "";
  }
};
