#pragma once

#include <fstream>
#include <numeric>
#include <sstream>
#include <unordered_map>

#include "commit.hxx"
#include "diff.hxx"
#include "dir_diff.hxx"
#include "file_status.hxx"
#include "revision.hxx"

using namespace std;
namespace fs = std::filesystem;

class Repository {
  fs::path path;

  fs::path revision_file(string type) {
    return lit_dir() / ("revision." + type);
  }

  optional<Revision> read_revision(string type) {
    auto path = revision_file(type);

    if (!fs::exists(path)) {
      return nullopt;
    }

    ifstream file(path);
    string id((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    id.pop_back();

    return optional(Revision(id));
  }

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

  fs::path revisions_dir() {
    return lit_dir() / "revisions";
  }

  fs::path working_copy_dir() {
    return lit_dir() / "working-copy";
  }

  void initialize() {
    fs::create_directory(lit_dir());
    fs::create_directory(revisions_dir());
    fs::create_directory(working_copy_dir());
  }

  bool is_initialized() {
    return fs::is_directory(lit_dir());
  }

  unordered_map<string, FileStatus> status() {
    return dir_diff(working_copy_dir(), dir());
  }

  void write_revision(string type, Revision revision) {
    ofstream file(revision_file(type));
    file << revision.id() << endl;
  }

  void write_revision(Revision revision) {
    write_revision("current", revision);

    auto latest = latest_revision();
    if (!latest || revision.number() > latest->number()) {
      write_revision("latest", revision);
    }
  }

  optional<Revision> current_revision() {
    return read_revision("current");
  }

  optional<Revision> latest_revision() {
    return read_revision("latest");
  }

  Revision next_revision() {
    auto rev = latest_revision();

    if (rev) {
      return Revision(rev.value().number() + 1);
    } else {
      return Revision(0);
    }
  }

  void checkout(Revision revision) {
    write_revision(revision);
  }

  Commit commit(Revision revision) {
    ifstream meta(revisions_dir() / revision.meta_filename());
    return Commit::deserialize(meta);
  }
};
