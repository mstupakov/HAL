#ifndef STUB_H__
#define STUB_H__

#include "gboard.h"

#include <iostream>

namespace hal {
  class Stub {
    public:
      Stub(GBoard*) {
        std::cerr << __PRETTY_FUNCTION__ << '\n';
    }
  };
}

#endif /* STUB_H__ */
