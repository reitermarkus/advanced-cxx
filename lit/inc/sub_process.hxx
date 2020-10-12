#pragma once

#include <array>
#include <cstdio>
#include <filesystem>
#include <stdexcept>

using namespace std;
namespace fs = std::filesystem;

class SubProcess {
  string command;
  fs::path cwd_;

  public:
  SubProcess(string command, fs::path cwd = fs::current_path()) {
    this->command = "'" + command + "'";
    this->cwd_ = cwd;
  }

  SubProcess& arg(string arg) {
    this->command += " '" + arg + "'";
    return *this;
  }

  SubProcess& cwd(fs::path cwd) {
    this->cwd_ = cwd;
    return *this;
  }

  pair<string, int> output() {
    string mode = "r";

    FILE* pipe = popen(("cd '" + string(this->cwd_) + "' && " + this->command + " 2>&1").c_str(), mode.c_str());
    if (!pipe) {
      throw runtime_error("popen failed");
    }

    array<char, 128> buffer;
    string output;
    while (fgets(buffer.data(), buffer.size(), pipe)) {
      output += buffer.data();
    }

    auto status = pclose(pipe);

    if (status == -1) {
      throw runtime_error("pclose failed");
    }

    return pair(output, WEXITSTATUS(status));
  }
};
