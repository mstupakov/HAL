#ifndef GBOARD_H__
#define GBOARD_H__

#include "rc.h"
#include "port.h"
#include "topology.h"

#include <type_traits>
#include <memory>

namespace hal {
  struct GBoard {
    virtual void Apply(port::Port) = 0;
  };

  template<typename T>
  struct GBoardImp : public GBoard {
    T m_switch;

    GBoardImp() : m_switch(this) {}

    virtual void Init(const Topology &t) {
      m_switch.Init(t);
    }

    virtual void Apply(port::Port port) override {
      m_switch.Apply(port);
    }
  };
}

#endif /* GBOARD_H__ */
