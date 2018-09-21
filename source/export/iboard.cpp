#include <iostream>
#include <list>

#include "common.h"
#include "iboard.h"
#include "board.h"
#include "gboard.h"
#include "dummy.h"

namespace hal {
  struct IBoard::Impl {
    std::shared_ptr<GBoard> m_board;

    Impl(BoardId id) {
      if (id == BoardId::E_DUMMY) {
        m_board = std::make_shared<bsp::Dummy>();

      } else {
        throw Exception(std::string(__PRETTY_FUNCTION__)
                        + ": Specified incorrect board ID: "
                        + std::to_string(to_underlying(id)));
      }
    }
  };

  IBoard::IBoard(BoardId id)
    : m_pimp(std::make_unique<Impl>(id)) {}

  IBoard::~IBoard() {}

  void IBoard::Init() {
    std::cerr << __PRETTY_FUNCTION__ << '\n';
    m_pimp->m_board->Init();
  }

  UIPort IBoard::GetIPort() {
    std::cerr << __PRETTY_FUNCTION__ << '\n';
    return std::make_shared<IPort>(m_pimp->m_board);
  }

  UIBoard IBoard::GetBoard(BoardId id) {
    return std::make_unique<IBoard>(id);
  }
} /* hal */
