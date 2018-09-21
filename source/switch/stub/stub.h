#ifndef STUB_H__
#define STUB_H__

#include "gboard.h"
#include "port.h"
#include "topology.h"

#include <iostream>

namespace hal {
  class Stub {
    public:
      Stub(GBoard*) {
        std::cerr << __PRETTY_FUNCTION__ << '\n';
      }

      void Init(const Topology &t) {}
      void Apply(port::Port) {}
  };
}

#endif /* STUB_H__ */
