#include "port.h"

#include <list>
#include <functional>
#include <iostream>
#include <sstream>

namespace hal {
  namespace port {

    Port::Port() 
      : m_speed(Speed::E_SPEED_100M),
        m_duplex(Duplex::E_DUPLEX_FULL),
        m_state(State::E_UP) {

      static int i = 1;
      m_physic_port = i++;
    }

    Port::operator unsigned() {
      return m_physic_port;
    };

    unsigned Port::GetPhysicPort() {
      return m_physic_port;
    }

    unsigned Port::GetLogicPort() {
      return m_logic_port;
    }

    std::tuple<Speed, Duplex, State> Port::GetStatus() {
      return std::make_tuple(m_speed, m_duplex, m_state);
    }

    void Port::SetSpeed(Speed speed, Duplex duplex) {
      m_speed = speed;
      m_duplex = duplex;
    }

    void Port::SetAdminMode(State state) {
      m_state = state;
    }

    void Port::Add(const Ids& ports) {
      for (auto p : ports) {
        std::cerr << __PRETTY_FUNCTION__ << ": " << p->m_physic_port << '\n';
      }
    }

    void Port::Sub(const Ids& ports) {
      for (auto p : ports) {
        std::cerr << __PRETTY_FUNCTION__ << ": " << p->m_physic_port << '\n';
      }
    }

    Ids Port::Get() {

    }
  }
}
