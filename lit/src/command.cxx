#include "command.hxx"
#include "command/checkout.hxx"
#include "command/commit.hxx"
#include "command/help.hxx"
#include "command/init.hxx"
#include "command/log.hxx"
#include "command/merge.hxx"
#include "command/show.hxx"
#include "command/status.hxx"

vector<unique_ptr<Command>> Command::list() {
  vector<unique_ptr<Command>> commands;

  commands.emplace_back(unique_ptr<Command>(new command::Checkout()));
  commands.emplace_back(unique_ptr<Command>(new command::Commit()));
  commands.emplace_back(unique_ptr<Command>(new command::Help()));
  commands.emplace_back(unique_ptr<Command>(new command::Init()));
  commands.emplace_back(unique_ptr<Command>(new command::Log()));
  commands.emplace_back(unique_ptr<Command>(new command::Merge()));
  commands.emplace_back(unique_ptr<Command>(new command::Show()));
  commands.emplace_back(unique_ptr<Command>(new command::Status()));

  return commands;
}
