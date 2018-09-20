#ifndef GBOARD_H__
#define GBOARD_H__

#include <type_traits>
#include <memory>

#include "rc.h"
#include "port.h"

namespace hal {
  struct GBoard {
    virtual void Init() = 0;

    virtual void SetPortAdmin() = 0;
    virtual void SetPortSpeed() = 0;
    virtual void SetPortDuplex() = 0;
    virtual void SetPortAutoNeg() = 0;
    virtual void SetPortSteering() = 0;

    virtual void Apply(port::Port) = 0;
  };

  template<typename T>
  struct GBoardImp : public GBoard {
    T m_switch;

    GBoardImp() : m_switch(this) {}

    virtual void Init() override {}

    virtual void SetPortAdmin() override {}
    virtual void SetPortSpeed() override {}
    virtual void SetPortDuplex() override {}
    virtual void SetPortAutoNeg() override {}
    virtual void SetPortSteering() override {}

    virtual void Apply(port::Port) override {}
  };
}

#endif /* GBOARD_H__ */
