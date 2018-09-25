#include "port.h"
#include "gboard.h"

#include <list>
#include <functional>
#include <iostream>
#include <sstream>
#include <algorithm>

namespace hal {
  namespace port {

    Port::Port(GBoard *board,
        unsigned physic_port, unsigned logic_port)
      : m_board(board),
        m_speed(Speed::E_SPEED_100M),
        m_duplex(Duplex::E_DUPLEX_FULL),
        m_state(State::E_UP),
        m_statistics() {

      m_physic_port = physic_port;
      m_logic_port = logic_port;
    }

    Port::operator unsigned() const {
      return m_physic_port;
    };

    unsigned Port::GetPhysicPort() const {
      return m_physic_port;
    }

    unsigned Port::GetLogicPort() const {
      return m_logic_port;
    }

    std::tuple<Speed, Duplex, State> Port::GetStatus() const {
      return std::make_tuple(m_speed, m_duplex, m_state);
    }

    void Port::SetSpeed(Speed speed, Duplex duplex) {
      m_speed = speed;
      m_duplex = duplex;

      m_board->Apply(*this);
    }

    void Port::SetAdminMode(State state) {
      m_state = state;
      m_board->Apply(*this);
    }

    MacStats Port::GetStatistics() const {
      return m_statistics;
    }

    void Port::Flush() {
      m_statistics = {};
    }

    void Port::Add(const Ids& ports) {
      for (auto p : ports) {
        std::cerr << __PRETTY_FUNCTION__ << " ++ : " << p->m_physic_port << '\n';
      }

      Ids ids;
      std::merge(std::begin(m_to_ports), std::end(m_to_ports),
          std::begin(ports), std::end(ports), std::inserter(ids, ids.begin()));
      m_to_ports = ids;

      for (auto p : m_to_ports) {
        std::cerr << __PRETTY_FUNCTION__ << " == : " << p->m_physic_port << '\n';
      }

      m_board->Apply(*this);
    }

    void Port::Sub(const Ids& ports) {
      for (auto p : ports) {
        std::cerr << __PRETTY_FUNCTION__ << " -- : " << p->m_physic_port << '\n';
      }

      Ids ids;
      std::set_difference(std::begin(m_to_ports), std::end(m_to_ports),
          std::begin(ports), std::end(ports), std::inserter(ids, ids.begin()));
      m_to_ports = ids;

      for (auto p : m_to_ports) {
        std::cerr << __PRETTY_FUNCTION__ << " == : " << p->m_physic_port << '\n';
      }

      m_board->Apply(*this);
    }

    Ids Port::Get() const {
      return m_to_ports;
    }
  }
}
