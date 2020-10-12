#pragma once

#include <chrono>
#include <iostream>
#include <optional>

#include "revision.hxx"

using namespace std;

class Commit {
  Revision revision_;
  optional<Revision> parent_a_;
  optional<Revision> parent_b_;
  chrono::time_point<chrono::system_clock> date_;
  string message_;

  public:
  Commit(Revision revision, optional<Revision> parent_a, optional<Revision> parent_b,
         chrono::time_point<chrono::system_clock> date, string message)
      : revision_(revision), parent_a_(parent_a), parent_b_(parent_b), date_(date), message_(message) {}
  Commit(Revision revision, optional<Revision> parent, chrono::time_point<chrono::system_clock> date, string message)
      : Commit(revision, parent, nullopt, date, message) {}

  string id() const {
    return this->revision_.id();
  }

  Revision revision() const {
    return this->revision_;
  }

  optional<Revision> parent_a() const {
    return this->parent_a_;
  }

  optional<Revision> parent_b() const {
    return this->parent_b_;
  }

  chrono::time_point<chrono::system_clock> date() const {
    return this->date_;
  }

  string message() const {
    return this->message_;
  }

  void pretty_print(ostream& os) const {
    os << "Commit: " << id() << endl;

    os << "Parents:";

    if (this->parent_a_) {
      os << " " << this->parent_a_.value().id();
    }

    if (this->parent_b_) {
      os << " " << this->parent_b_.value().id();
    }

    os << endl;

    os << "Date: " << chrono::system_clock::to_time_t(this->date_) << endl;
    os << endl;
    os << this->message_ << endl;
  }

  void serialize(ostream& os) const {
    os << id() << endl;
    if (this->parent_a_) {
      os << this->parent_a_.value().id();
    }
    os << endl;
    if (this->parent_b_) {
      os << this->parent_b_.value().id();
    }
    os << endl;
    os << chrono::system_clock::to_time_t(this->date_) << endl;
    os << this->message_ << endl;
  }

  static Commit deserialize(istream& is) {
    string line;

    getline(is, line);
    auto revision = Revision(line);

    getline(is, line);
    auto parent_a = line.empty() ? nullopt : optional(Revision(line));

    getline(is, line);
    auto parent_b = line.empty() ? nullopt : optional(Revision(line));

    getline(is, line);
    auto date = chrono::system_clock::from_time_t(stoul(line));

    string message((istreambuf_iterator<char>(is)), istreambuf_iterator<char>());
    message.pop_back();

    return Commit(revision, parent_a, parent_b, date, message);
  }
};

ostream& operator<<(ostream& os, const Commit& commit)

{
  commit.pretty_print(os);
  return os;
}
