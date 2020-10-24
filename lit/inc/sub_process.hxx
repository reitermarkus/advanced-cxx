#pragma once

#include <array>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <stdexcept>

#include "fs.hxx"

namespace lit {

const auto MAX_LINE_LENGTH = 512;

using namespace std;

class SubProcess {
  string command;

  public:
  explicit SubProcess(const string& command) {
    this->command = "'" + command + "'";
  }

  SubProcess& arg(const string& arg) {
    this->command += " '" + arg + "'";
    return *this;
  }

  pair<string, int> output() {
    string mode = "r";

    FILE* pipe = popen((this->command + " 2>&1").c_str(), mode.c_str());
    if (pipe == nullptr) {
      throw runtime_error("popen failed");
    }

    array<char, MAX_LINE_LENGTH> buffer{};
    string output;
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
      output += buffer.data();
    }

    auto status = pclose(pipe);

    if (status == -1) {
      throw runtime_error("pclose failed");
    }

    return pair(output, WEXITSTATUS(status)); // NOLINT(hicpp-signed-bitwise)
  }
};

} // namespace lit
