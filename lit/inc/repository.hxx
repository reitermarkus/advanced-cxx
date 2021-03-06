#pragma once

#include <deque>
#include <fstream>
#include <numeric>
#include <sstream>
#include <unordered_map>

#include "commit.hxx"
#include "diff.hxx"
#include "dir_diff.hxx"
#include "file_status.hxx"
#include "fs.hxx"
#include "patch.hxx"
#include "revision.hxx"

namespace lit {

using namespace std;

class Repository {
  fs::path path;

  fs::path revision_file(const string& type) {
    return lit_dir() / ("revision." + type);
  }

  optional<Revision> read_revision(const string& type) {
    const auto path = revision_file(type);

    if (!fs::exists(path)) {
      return nullopt;
    }

    ifstream file(path);
    string id((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    id.pop_back();

    return optional(Revision(id));
  }

  void remove_merge_revision() {
    const auto path = revision_file("merge");

    if (fs::exists(path)) {
      fs::remove(path);
    }
  }

  void write_revision(const string& type, const Revision& revision) {
    ofstream file(revision_file(type));
    file << revision.id() << endl;
  }

  public:
  explicit Repository() {
    this->path = fs::current_path();
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

  void initialize() {
    fs::create_directory(lit_dir());
    fs::create_directory(revisions_dir());
  }

  bool is_initialized() {
    return fs::is_directory(lit_dir());
  }

  unordered_map<string, FileStatus> status() {
    const auto temp_repo_dir = checkout_temp_directory(current_revision());
    return dir_diff(*temp_repo_dir, dir());
  }

  void write_current_revision(const Revision& revision) {
    write_revision("current", revision);

    const auto latest = latest_revision();
    if (!latest || revision.number() > latest->number()) {
      write_revision("latest", revision);
    }

    remove_merge_revision();
  }

  void write_merge_revision(const Revision& revision) {
    write_revision("merge", revision);
  }

  optional<Revision> merge_revision() {
    return read_revision("merge");
  }

  optional<Revision> current_revision() {
    return read_revision("current");
  }

  optional<Revision> latest_revision() {
    return read_revision("latest");
  }

  Revision next_revision() {
    const auto rev = latest_revision();

    if (rev) {
      return Revision(rev.value().number() + 1);
    }

    return Revision(0);
  }

  Commit commit(const Revision& revision) {
    ifstream meta(revisions_dir() / revision.meta_filename());
    return Commit::deserialize(meta);
  }

  void create_commit(const Commit& commit, optional<Revision> current_revision) {
    const auto temp_dir = fs::create_temp_directory();

    const auto meta_file_path = *temp_dir / commit.revision().meta_filename();
    const auto patch_file_path = *temp_dir / commit.revision().patch_filename();

    ofstream meta_file(meta_file_path);
    commit.serialize(meta_file);
    meta_file.close();

    const auto temp_repo_dir = checkout_temp_directory(current_revision);
    const auto patch = Patch::create(*temp_repo_dir, dir(), patch_file_path);

    fs::rename(meta_file_path, revisions_dir() / commit.revision().meta_filename());
    fs::rename(patch.path(), revisions_dir() / commit.revision().patch_filename());

    write_current_revision(commit.revision());
  }

  fs::temp_path checkout_temp_directory(const optional<Revision>& revision) {
    auto temp_repo_dir = fs::create_temp_directory();

    deque<Revision> revisions;

    auto current_revision = revision;
    while (current_revision) {
      const auto r = current_revision.value();
      revisions.push_front(r);
      current_revision = commit(r).parent_a();
    }

    for (const auto& r: revisions) {
      Patch patch(revisions_dir() / r.patch_filename());
      patch.apply(*temp_repo_dir);
    }

    return temp_repo_dir;
  }

  void checkout(const Revision& revision) {
    const auto repo_dir = dir();
    const auto temp_repo_dir = checkout_temp_directory(optional(revision));

    const auto repo_lit_dir = lit_dir();
    for (const auto& entry: fs::repository_iterator(repo_dir)) {
      fs::remove_all(entry.path());
    }

    const auto temp_dir_parts = fs::count_path_parts(*temp_repo_dir);

    for (const auto& entry: fs::repository_iterator(*temp_repo_dir)) {
      const auto& path = entry.path();
      const auto relative_path = fs::strip_path_prefix_parts(path, temp_dir_parts);

      fs::rename(path, repo_dir / relative_path);
    }

    write_current_revision(revision);
  }

  optional<Revision> merge_base_opt(optional<Revision> revision_a, optional<Revision> revision_b) {
    if (revision_a && revision_b) {
      return merge_base(revision_a.value(), revision_b.value());
    }

    return revision_a;
  }

  optional<Revision> merge_base(const Revision& revision_a, const Revision& revision_b) {
    const auto next_parent = [&](const Revision& revision) {
      const auto c = commit(revision);

      if (c.parent_b()) {
        return c.parent_b();
      }

      return c.parent_a();
    };

    auto next_a = next_parent(revision_a);
    auto next_b = next_parent(revision_b);

    while (next_a && next_b) {
      if (*next_a > *next_b) {
        next_a = next_parent(*next_a);
      }

      if (*next_b > *next_a) {
        next_b = next_parent(*next_b);
      }

      if (*next_a == *next_b) {
        return *next_a;
      }
    }

    return nullopt;
  }
};

} // namespace lit
