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

      void Init() {
        GBoardImp<Stub>::Init({ {1, 1},
                                {2, 2},
                                {3, 3},
                                {4, 4},
                                {5, 5}
        });
      }
    };
  }
}

#endif /* DUMMY_H__ */
