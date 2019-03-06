#ifndef IPORT_H__
#define IPORT_H__

#include "common/common.h"
#include "common/port.h"

#include <memory>
#include <list>
#include <map>

namespace hal {
  class IPort {
    std::shared_ptr<GBoard> m_board;
    std::list<std::shared_ptr<port::Port>> m_ports;

    public:
      IPort(std::shared_ptr<GBoard>);

      void Subscribe(port::Notify);
      std::shared_ptr<port::Port> GetPort(unsigned);

      using value_type = decltype(m_ports)::value_type;
      using iterator   = decltype(m_ports)::iterator;
      using size_type  = decltype(m_ports)::size_type;

      size_type size() const noexcept;
      size_type max_size() const noexcept;

      iterator begin();
      iterator end();
  };
}

#endif /* IPORT_H__ */
