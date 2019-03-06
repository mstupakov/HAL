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
        PAccessor(port).opr_cur() = PAccessor(port).cfg_cur();
      }

      void Update(port::Port &port) {
        PAccessor(port).opr_cur() = PAccessor(port).cfg_cur();

        PAccessor(port).stats().rx_good_octets       += 1;
        PAccessor(port).stats().tx_good_octets       += 5;
        PAccessor(port).stats().total_64_pkts        += 7;
        PAccessor(port).stats().rx_good_uc_pkts      += 8;
        PAccessor(port).stats().tx_good_uc_pkts      += 9;
        PAccessor(port).stats().total_1024_1518_pkts += 5;
      }
  };
}

#endif /* STUB_H__ */
