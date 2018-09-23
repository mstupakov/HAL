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

      void Init(const Topology &t) {
        std::cerr << __PRETTY_FUNCTION__ << '\n';
      }

      void Apply(port::Port) {
        std::cerr << __PRETTY_FUNCTION__ << '\n';
      }
  };
}

#endif /* STUB_H__ */
