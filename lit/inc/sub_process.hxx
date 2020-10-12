#include <array>
#include <cstdio>

#include <stdexcept>

using namespace std;

class SubProcess {
  string command;

  public:
  SubProcess(string command) {
    this->command = "'" + command + "'";
  }

  SubProcess& arg(string arg) {
    this->command += " '" + arg + "'";
    return *this;
  }

  pair<string, int> output() {
    string mode = "r";

    FILE* pipe = popen((this->command + " 2>&1").c_str(), mode.c_str());
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
