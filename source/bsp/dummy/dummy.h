#ifndef DUMMY_H__
#define DUMMY_H__

#include "gboard.h"
#include "stub.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

namespace hal {
  namespace bsp {

    struct Dummy : GBoardImp<Stub> {
      std::thread m_thread;
      std::atomic<bool> m_run;

      Dummy() : m_run(true) {
        std::clog << __PRETTY_FUNCTION__ << '\n';
      }

     ~Dummy() {
        std::clog << __PRETTY_FUNCTION__ << '\n';

        m_run = false;
        m_thread.join();
      }

      void Init() {
        GBoardImp<Stub>::Init({ {0, 0}, {1, 1},
                                {2, 2}, {3, 3},
                                {4, 4}, {5, 5},
        });

        m_thread = std::move(std::thread([this] () {
            using namespace std::chrono;

            while (m_run) {
              Update(); std::this_thread::sleep_for(1s);
            }
        }));
      }
    };
  }
}

#endif /* DUMMY_H__ */
