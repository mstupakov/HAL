#ifndef STUB_H__
#define STUB_H__

#include "gboard.h"
#include "port.h"
#include "topology.h"
#include "accessor.h"

#include <iostream>

namespace hal {
  class Stub {
    public:
      Stub(GBoard*) {
        std::clog << __PRETTY_FUNCTION__ << '\n';
      }

      void Init(const Topology&) {
        std::clog << __PRETTY_FUNCTION__ << '\n';
      }

      void Apply(port::Port &port) {
        std::clog << __PRETTY_FUNCTION__ << '\n';
        PAccessor(port).cfg_cur() = PAccessor(port).cfg_new();
      }

      void Get(port::Port &port) {
        std::clog << __PRETTY_FUNCTION__ << '\n';
        PAccessor(port).opr_cur() = PAccessor(port).cfg_cur();
      }
  };
}

#endif /* STUB_H__ */
