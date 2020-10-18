#pragma once

#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;

template <typename T> ostream& operator<<(ostream& os, vector<T> vec) {
  os << "[";

  for (auto i = 0; i < vec.size(); i++) {
    if (i > 0) {
      cout << ", ";
    }

    cout << vec[i];
  }

  os << "]";

  return os;
}
