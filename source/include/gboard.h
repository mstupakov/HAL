#ifndef GBOARD_H__
#define GBOARD_H__

#include "rc.h"
#include "port.h"
#include "topology.h"

#include <type_traits>
#include <memory>
#include <iostream>

namespace hal {
  struct GBoard {
    virtual void Init() = 0;

    virtual void Apply(port::Port) = 0;
    virtual void Subscribe(port::Notify) = 0;

    protected:
      port::Port CreatePort(unsigned physic_port,
          unsigned logic_port) {
        return port::Port(physic_port, logic_port);
      }
  };

  template<typename T>
  struct GBoardImp : public GBoard {
    T m_switch;
    std::list<port::Notify> m_port_notify;

    GBoardImp() : m_switch(this) {}

    virtual void Init(const Topology &t) {
      m_switch.Init(t);

      for (auto p : t) {
        Notify(port::Event::E_CREATE,
            CreatePort(p.physic_port, p.logic_port));
      }
    }

    virtual void Apply(port::Port port) override {
      m_switch.Apply(port);
    }

    virtual void Subscribe(port::Notify notify) override {
      m_port_notify.push_back(notify);
    }

    protected:
      void Notify(port::Event e, port::Port p) {
        std::for_each(std::begin(m_port_notify), std::end(m_port_notify),
            [e, p](auto &&f) { f(e, {std::make_shared<port::Port>(p)}); });
      }
  };
}

#endif /* GBOARD_H__ */
