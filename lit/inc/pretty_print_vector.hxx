#pragma once

#include <algorithm>
#include <deque>
#include <iterator>
#include <vector>

using namespace std;

template <typename T> ostream& operator<<(ostream& os, vector<T> vec) {
  os << "[";

  for (size_t i = 0; i < vec.size(); i++) {
    if (i > 0) {
      cout << ", ";
    }

    cout << vec[i];
  }

  os << "]";

  return os;
}

template <typename T> ostream& operator<<(ostream& os, deque<T> vec) {
  os << "[";

  for (size_t i = 0; i < vec.size(); i++) {
    if (i > 0) {
      cout << ", ";
    }

    cout << vec[i];
  }

  os << "]";

  return os;
}
