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
      : m_board(board), m_statistics(), m_cfg_cur(),
        m_cfg_new(), m_opr_cur(), m_opr_old() {

      m_physic_port = physic_port;
      m_logic_port = logic_port;
    }

    Port::operator unsigned() const {
      return m_logic_port;
    };

    unsigned Port::GetPhysicPort() const {
      return m_physic_port;
    }

    std::tuple<Speed, Duplex, State> Port::GetStatus() {
      m_board->Get(*this);
      return m_opr_cur;
    }

    std::tuple<Speed, Duplex, State> Port::GetCfg() {
      return m_cfg_cur;
    }

    void Port::SetSpeed(Speed speed, Duplex duplex) {
      m_cfg_new = std::make_tuple(speed, duplex,
          std::get<State>(m_cfg_cur));

      m_board->Apply(*this);
    }

    void Port::SetAdminMode(State state) {
      m_cfg_new = std::make_tuple(std::get<Speed>(m_cfg_cur),
          std::get<Duplex>(m_cfg_cur), state);

      m_board->Apply(*this);
    }

    MacStats Port::GetStatistics() {
      m_board->Get(*this);
      return m_statistics;
    }

    void Port::Flush() {
      m_statistics = {};
    }

    void Port::Add(const Ids& ports) {
      for (auto p : ports) {
        std::clog << __PRETTY_FUNCTION__ << " ++ : " << p->m_physic_port << '\n';
      }

      Ids ids;
      std::merge(std::begin(m_to_ports), std::end(m_to_ports),
          std::begin(ports), std::end(ports), std::inserter(ids, ids.begin()));
      m_to_ports = ids;

      for (auto p : m_to_ports) {
        std::clog << __PRETTY_FUNCTION__ << " == : " << p->m_physic_port << '\n';
      }

      m_board->Apply(*this);
    }

    void Port::Sub(const Ids& ports) {
      for (auto p : ports) {
        std::clog << __PRETTY_FUNCTION__ << " -- : " << p->m_physic_port << '\n';
      }

      Ids ids;
      std::set_difference(std::begin(m_to_ports), std::end(m_to_ports),
          std::begin(ports), std::end(ports), std::inserter(ids, ids.begin()));
      m_to_ports = ids;

      for (auto p : m_to_ports) {
        std::clog << __PRETTY_FUNCTION__ << " == : " << p->m_physic_port << '\n';
      }

      m_board->Apply(*this);
    }

    Ids Port::Get() const {
      return m_to_ports;
    }
  }
}
