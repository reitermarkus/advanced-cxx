#include "revision.hxx"

ostream& operator<<(ostream& os, Revision revision) {
  os << "Revision(" << revision.number() << ")";
  return os;
}
