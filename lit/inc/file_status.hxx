#pragma once

namespace lit {

enum FileStatus {
  Added,
  Modified,
  Deleted,
};

char to_char(const FileStatus& file_status) {
  switch (file_status) {
    case Added:
      return 'A';
    case Modified:
      return 'M';
    case Deleted:
      return 'D';
  }
}

}
