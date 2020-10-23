#include "revision.hxx"

namespace lit {

ostream& operator<<(ostream& os, const Revision& revision) {
  os << "Revision(" << revision.number() << ")";
  return os;
}

}
