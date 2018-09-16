#include <iostream>
#include <list>

#include "common.h"
#include "iport.h"
#include "board.h"
//#include "gboard.h"

namespace hal {
  IPort::IPort(std::shared_ptr<GBoard> board)
    :  m_board(board) {
    std::cerr << __PRETTY_FUNCTION__ << '\n';
  }
} /* hal */
