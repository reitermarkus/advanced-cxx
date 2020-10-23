#include "revision.hxx"

ostream& operator<<(ostream& os, const Revision& revision) {
  os << "Revision(" << revision.number() << ")";
  return os;
}
