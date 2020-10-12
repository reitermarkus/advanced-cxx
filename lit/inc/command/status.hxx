#pragma once

#include <filesystem>
#include <functional>
#include <iostream>
#include <numeric>

#include "command.hxx"
#include "repository.hxx"

using namespace std;
namespace fs = std::filesystem;

size_t count_path_parts(fs::path path) {
  return accumulate(path.begin(), path.end(), 0, [](size_t acc, fs::path _) { return acc + 1; });
}

fs::path strip_path_prefix_parts(fs::path path, size_t n) {
  auto begin = path.begin();
  advance(begin, n);
  fs::path init = *begin;
  advance(begin, 1);
  return reduce(begin, path.end(), init, divides<fs::path>());
}

namespace command {

class Status: public Command {
  public:
  string name() const override {
    return "status";
  }
  string description() const override {
    return "Show the working tree status.";
  }

  int run(vector<string>& arguments) override {
    auto repo = Repository();

    auto repo_path_parts = count_path_parts(repo.dir());

    auto status = [&](fs::directory_entry entry) {
      auto path = entry.path();

      auto new_path = strip_path_prefix_parts(entry.path(), repo_path_parts);

      cout << "A  " << string(new_path) << endl;
    };

    for (auto& entry: fs::directory_iterator(repo.dir())) {
      auto path = entry.path();

      if (path == repo.lit_dir()) {
        continue;
      } else if (entry.is_directory()) {
        for (auto& recursive_entry: fs::recursive_directory_iterator(path)) {
          status(recursive_entry);
        }
      } else {
        status(entry);
      }
    }

    return 0;
  }
};

}
