#ifndef IPORT_H__
#define IPORT_H__

#include "common/common.h"
#include "common/port.h"

#include <memory>
#include <list>
#include <map>

namespace hal {
  class IPort {
    public:
    std::shared_ptr<GBoard> m_board;
    //std::list<std::shared_ptr<port::Port>> m_ports = {
    //                                  std::make_shared<port::Port>(), 
    //                                  std::make_shared<port::Port>(),
    //                                  std::make_shared<port::Port>()
    std::list<std::shared_ptr<port::Port>> m_ports = {
                                      std::make_shared<port::Port>(), 
                                      std::make_shared<port::Port>(),
                                      std::make_shared<port::Port>()
    };

    public:
      IPort(std::shared_ptr<GBoard>);

      void Subscribe(port::Notify);
      std::shared_ptr<port::Port> GetPort(unsigned port);

      decltype(m_ports)::iterator begin();
      decltype(m_ports)::iterator end();
  };
}

#endif /* IPORT_H__ */
