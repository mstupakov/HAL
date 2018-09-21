#ifndef TOPOLOGY_H__
#define TOPOLOGY_H__

#include <list>

namespace hal {
  struct PortCfg {
    unsigned physic_port;
    unsigned logic_port;
  };

  using Topology = std::list<PortCfg>;
}

#endif /* TOPOLOGY_H__ */
