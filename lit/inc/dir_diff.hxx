#pragma once

#include <unordered_map>

#include "diff.hxx"
#include "file_status.hxx"
#include "fs.hxx"

unordered_map<string, FileStatus> dir_diff(const fs::path& dir_a, const fs::path& dir_b);
