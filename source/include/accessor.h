#ifndef ACCESSOR_H__
#define ACCESSOR_H__

#include "port.h"
#include "gboard.h"

namespace hal {
  struct PAccessor {
    port::Port& m_port;

    PAccessor(port::Port& port) : m_port(port) {}
    PAccessor(const port::Port& port)
      : m_port(const_cast<port::Port&> (port)) {}

    static port::Port Create(GBoard *b,
        unsigned physic_port, unsigned logic_port) {
      return port::Port(b, physic_port, logic_port);
    }

    decltype(m_port.m_cfg_cur)& cfg_cur() {
      return m_port.m_cfg_cur;
    }

    decltype(m_port.m_cfg_new)& cfg_new() {
      return m_port.m_cfg_new;
    }

    decltype(m_port.m_opr_cur)& opr_cur() {
      return m_port.m_opr_cur;
    }

    decltype(m_port.m_opr_old)& opr_old() {
      return m_port.m_opr_old;
    }

    decltype(m_port.m_statistics)& stats() {
      return m_port.m_statistics;
    }

    decltype(m_port.m_logic_port)& logic() {
      return m_port.m_logic_port;
    }

    decltype(m_port.m_to_ports)& to_ports() {
      return m_port.m_to_ports;
    }
  };
}

#endif /* ACCESSOR_H__ */
