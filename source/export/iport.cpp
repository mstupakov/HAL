#include <iostream>
#include <list>
#include <algorithm>

#include "common.h"
#include "iport.h"
#include "board.h"
#include "gboard.h"

namespace hal {
  IPort::IPort(std::shared_ptr<GBoard> board)
    :  m_board(board) {
    std::clog << __PRETTY_FUNCTION__ << '\n';
    m_board->Subscribe([this](port::Event e, const port::Ids &ports) {
      if (e == port::Event::E_CREATE) {
        std::for_each(std::begin(ports), std::end(ports),
            [this](auto p) { this->m_ports.push_back(p); });
      }
    });
  }

  void IPort::Subscribe(port::Notify notify) {
    m_board->Subscribe(notify);
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

  IPort::size_type IPort::size() const noexcept {
    return m_ports.size();
  }

  IPort::size_type IPort::max_size() const noexcept {
    return m_ports.max_size();
  }

  IPort::iterator IPort::begin() {
    return m_ports.begin();
  }

  IPort::iterator IPort::end() {
    return m_ports.end();
  }
} /* hal */
