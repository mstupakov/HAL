#ifndef DUMMY_H__
#define DUMMY_H__

#include "gboard.h"
#include "stub.h"

#include <iostream>

namespace hal {
  namespace bsp {

    struct Dummy : GBoardImp<Stub> {
      Dummy() {
        std::cerr << __PRETTY_FUNCTION__ << '\n';
      }

     ~Dummy() {
        std::cerr << __PRETTY_FUNCTION__ << '\n';
      }
    };
  }
}

#endif /* DUMMY_H__ */
