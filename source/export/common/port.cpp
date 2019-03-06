#include "port.h"
#include "gboard.h"
#include "accessor.h"

#include <list>
#include <functional>
#include <iostream>
#include <sstream>
#include <algorithm>

namespace hal {
  namespace port {

    Port::Port(GBoard *board,
        unsigned physic_port, unsigned logic_port)
      : m_board(board), m_physic_port(physic_port),
        m_logic_port(logic_port) {

      m_cfg_cur = std::make_tuple(
          Speed::E_SPEED_100M, Duplex::E_DUPLEX_FULL, State::E_UP);

      m_cfg_new = std::make_tuple(
          Speed::E_SPEED_100M, Duplex::E_DUPLEX_FULL, State::E_DOWN);

      m_opr_cur = m_cfg_new;
      m_opr_old = m_cfg_cur;
    }

    Port::operator unsigned() const {
      std::lock_guard<std::recursive_mutex> lock(m_lock);
      return m_logic_port;
    };

    unsigned Port::GetPhysicPort() const {
      std::lock_guard<std::recursive_mutex> lock(m_lock);
      return m_physic_port;
    }

    std::tuple<Speed, Duplex, State> Port::GetStatus() {
      std::lock_guard<std::recursive_mutex> lock(m_lock);

      m_board->Get(*this);
      return m_opr_cur;
    }

    std::tuple<Speed, Duplex, State> Port::GetCfg() {
      std::lock_guard<std::recursive_mutex> lock(m_lock);
      return m_cfg_cur;
    }

    void Port::SetSpeed(Speed speed, Duplex duplex) {
      std::lock_guard<std::recursive_mutex> lock(m_lock);

      m_cfg_new = std::make_tuple(speed, duplex,
          std::get<State>(m_cfg_cur));

      m_board->Apply(*this);
    }

    void Port::SetAdminMode(State state) {
      std::lock_guard<std::recursive_mutex> lock(m_lock);

      m_cfg_new = std::make_tuple(std::get<Speed>(m_cfg_cur),
          std::get<Duplex>(m_cfg_cur), state);

      m_board->Apply(*this);
    }

    MacStats Port::GetStatistics() {
      std::lock_guard<std::recursive_mutex> lock(m_lock);

      m_board->Get(*this);
      return m_statistics;
    }

    void Port::Flush() {
      std::lock_guard<std::recursive_mutex> lock(m_lock);
      m_statistics = {};
    }

    void Port::Add(const Ids& ports) {
      std::lock_guard<std::recursive_mutex> lock(m_lock);

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
      std::lock_guard<std::recursive_mutex> lock(m_lock);

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
      std::lock_guard<std::recursive_mutex> lock(m_lock);
      return m_to_ports;
    }

    std::ostream& operator<<(std::ostream &os, Port &port) {
      PAccessor p(port);

      Speed speed; Duplex duplex; State state;
      std::tie(speed, duplex, state) = p.opr_cur();

      Speed cfg_speed; Duplex cfg_duplex; State cfg_state;
      std::tie(cfg_speed, cfg_duplex, cfg_state) = p.cfg_cur();

      std::string ports;

      for (auto to : p.to_ports()) {
        ports += " " + std::to_string(PAccessor(*to).physic()) + " ";
      }

      std::stringstream ss;
      ss << "\n ===- Port -===";
      ss << "\n  - physic : " << p.physic();
      ss << "\n  - logic  : " << p.logic();
      ss << "\n";
      ss << "\n = Configuration:";
      ss << "\n  - speed  : " << cfg_speed;
      ss << "\n  - duplex : " << cfg_duplex;
      ss << "\n  - admin  : " << cfg_state;
      ss << "\n";
      ss << "\n = Operational:";
      ss << "\n  - speed  : " << speed;
      ss << "\n  - duplex : " << duplex;
      ss << "\n  - link   : " << state;

      ss << "\n to {" << ports << "}";
      ss << "\n\n";

      return os << ss.str();
    }
  }
}
