#pragma once

#include <unordered_map>

#include "diff.hxx"
#include "file_status.hxx"
#include "fs.hxx"

unordered_map<string, FileStatus> dir_diff(fs::path dir_a, fs::path dir_b);
