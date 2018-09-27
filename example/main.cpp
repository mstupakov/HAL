#include <iostream>
#include <string>
#include <map>
#include <chrono>
#include <thread>

#include "iboard.h"
#include "iport.h"

using namespace std::chrono_literals;

int main(int argc, char *argv[]) {
  try {
    hal::BoardId id = hal::BoardId::E_DUMMY;

    std::map<std::string, hal::BoardId> map = {
      { "dummy", hal::BoardId::E_DUMMY }
    };

    if (argc > 1) {
      id = map.at(argv[1]);
    }

    hal::UIBoard board = hal::IBoard::GetBoard(id);
    hal::UIPort ports = board->GetIPort();

    ports->Subscribe([](hal::port::Event e, const hal::port::Ids &ports) {
      for (auto p : ports) {
        std::clog << __PRETTY_FUNCTION__ << ": Event: " << e << '\n';
        std::clog << __PRETTY_FUNCTION__ << '\n' << *p << '\n';
      }
    });

    board->Init();

    for (auto p : *ports) {
      std::clog << __PRETTY_FUNCTION__ << ": " << *p << '\n';
    }

    auto p0 = ports->GetPort(0);
    std::clog << __PRETTY_FUNCTION__ << ": " << *p0 << '\n';

    auto p1 = ports->GetPort(1);
    std::clog << __PRETTY_FUNCTION__ << ": " << *p1 << '\n';

    auto p2 = ports->GetPort(2);
    std::clog << __PRETTY_FUNCTION__ << ": " << *p2 << '\n';

    p0->Add({p1, p2});
    p0->Sub({p1, p2});
    p0->Add({p1, p1, p1});

    auto to_ports = p0->Get();
    for (auto p : to_ports) {
      std::clog << __PRETTY_FUNCTION__ << " == : " << *p << '\n';
    }

    auto cfg = p0->GetCfg();

    p1->SetSpeed(std::get<hal::port::Speed>(cfg),
                 std::get<hal::port::Duplex>(cfg));
    p1->SetAdminMode(std::get<hal::port::State>(cfg));

    std::clog << *p0 << '\n';
    std::clog << p0->GetStatistics();

    std::this_thread::sleep_for(5s);
    p1->SetAdminMode(hal::port::State::E_UP);
    p2->SetAdminMode(hal::port::State::E_UP);

    std::this_thread::sleep_for(5s);
    std::clog << *p1 << '\n';

    std::this_thread::sleep_for(5s);
    p1->SetAdminMode(hal::port::State::E_DOWN);
    p2->SetAdminMode(hal::port::State::E_DOWN);

    std::this_thread::sleep_for(5s);
    std::clog << *p1 << '\n';

    std::this_thread::sleep_for(5s);
    p1->SetAdminMode(hal::port::State::E_UP);
    p2->SetAdminMode(hal::port::State::E_UP);

    std::this_thread::sleep_for(5s);
    std::clog << *p1 << '\n';

    std::this_thread::sleep_for(5s);
    std::clog << p1->GetStatistics();
  } catch (hal::Exception &e) {
    std::clog << "Error: " << e.what() << '\n';
    std::clog << " - RC: " << e.GetReason() << '\n';
  }

  return 0;
}
