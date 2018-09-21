#include <iostream>
#include <list>

#include "common.h"
#include "iport.h"
#include "board.h"
#include "gboard.h"

namespace hal {
  IPort::IPort(std::shared_ptr<GBoard> board)
    :  m_board(board) {
    std::cerr << __PRETTY_FUNCTION__ << '\n';
  }

  void IPort::Subscribe(port::Notify) {

  }

  std::shared_ptr<port::Port> IPort::GetPort(unsigned port) {
    for (auto p : m_ports) {
      if (p->GetPhysicPort() == port) {
        return p;
      }
    }

    throw Exception(std::string(__PRETTY_FUNCTION__)
                    + ": Specified port ID doesn't exist: "
                    + std::to_string(port));
  }

  decltype(IPort::m_ports)::iterator IPort::begin() {
    return m_ports.begin();
  }

  decltype(IPort::m_ports)::iterator IPort::end() {
    return m_ports.end();
  }
} /* hal */
