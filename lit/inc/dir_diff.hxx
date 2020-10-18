#pragma once

#include <numeric>
#include <unordered_map>

#include "diff.hxx"
#include "file_status.hxx"
#include "fs.hxx"

size_t count_path_parts(fs::path path);
fs::path strip_path_prefix_parts(fs::path path, size_t n);

unordered_map<string, FileStatus> dir_diff(fs::path dir_a, fs::path dir_b);
