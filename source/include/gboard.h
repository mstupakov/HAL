#ifndef GBOARD_H__
#define GBOARD_H__

#include "rc.h"
#include "port.h"
#include "topology.h"
#include "accessor.h"

#include <type_traits>
#include <list>
#include <map>
#include <memory>
#include <iostream>
#include <algorithm>

namespace hal {
  struct GBoard {
    virtual void Init() = 0;

    virtual void Apply(port::Port&) = 0;
    virtual void Get(port::Port&) = 0;

    virtual void Subscribe(port::Notify) = 0;
  };

  template<typename T>
  struct GBoardImp : public GBoard {
    T m_switch;
    std::list<port::Notify> m_port_notify;
    std::map<unsigned, std::shared_ptr<port::Port>> m_ports;

    GBoardImp() : m_switch(this) {}

    virtual void Init(const Topology &t) {
      m_switch.Init(t);

      for (auto p : t) {
        m_ports[p.logic_port] =
            PAccessor::Create(this, p.physic_port, p.logic_port);

        Apply(*m_ports[p.logic_port]);
      }

      for (auto p : m_ports) {
        Notify(port::Event::E_CREATE, *p.second);
      }
    }

    virtual void Apply(port::Port &port) override {
      m_switch.Apply(port);
    }

    virtual void Get(port::Port &port) override {
      m_switch.Get(port);
    }

    virtual void Subscribe(port::Notify notify) override {
      m_port_notify.push_back(notify);
    }

    void Update(port::Port &port) {
      m_switch.Update(port);
    }

    protected:
      void Update() {
        for (auto pair : m_ports) {
          auto &port = *pair.second;
          PAccessor p(port);

          std::lock_guard<std::recursive_mutex> lock(p.lock());
          Update(port);

          if (p.opr_cur() != p.opr_old()) {
            p.opr_old() = p.opr_cur();
            std::clog << __PRETTY_FUNCTION__ << ": " << port << '\n';
            Notify(port::Event::E_LINK_CHANGE, *m_ports[p.logic()]);
          }
        }
      }

      void Notify(port::Event e, port::Port &p) {
        std::for_each(std::begin(m_port_notify), std::end(m_port_notify),
            [e, &p, this](auto &&f) {
                f(e, {this->m_ports.at(PAccessor(p).logic())});
            });
      }
  };
}

#endif /* GBOARD_H__ */
