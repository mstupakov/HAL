#ifndef IBOARD_H__
#define IBOARD_H__

#include <memory>

#include "common/common.h"
#include "common/board.h"
#include "iport.h"

namespace hal {
  struct IBoard; using UIBoard = std::unique_ptr<IBoard>;
  struct IPort; using UIPort = std::shared_ptr<IPort>;

  struct IBoard {
    void Init();

    UIPort GetIPort();
    static UIBoard GetBoard(BoardId);

    IBoard(BoardId);
   ~IBoard();

    private:
      struct Impl;
      Pimp<Impl> m_pimp;
  };
}

#endif /* IBOARD_H__ */
