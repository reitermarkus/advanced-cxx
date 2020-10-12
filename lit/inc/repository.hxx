#pragma once

using namespace std;
namespace fs = std::filesystem;

class Repository {
  fs::path path;

  public:
  Repository(fs::path path = fs::current_path()) {
    this->path = path;
  }

  fs::path dir() {
    return this->path;
  }

  fs::path lit_dir() {
    return this->path / ".lit";
  }

  void initialize() {
    fs::create_directory(lit_dir());
  }

  bool is_initialized() {
    return fs::is_directory(lit_dir());
  }
};
