#ifndef TOPOLOGY_H__
#define TOPOLOGY_H__

#include <list>

namespace hal {
  struct PortCfg {
    unsigned port;
  };

  using Topology = std::list<PortCfg>;
}

#endif /* TOPOLOGY_H__ */
