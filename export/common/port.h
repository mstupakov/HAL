#ifndef PORT_H__
#define PORT_H__

#include "common.h"

#include <list>
#include <functional>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>

namespace hal {
  namespace port {
    struct Port;

    enum class Event {
      E_CREATE,
      E_REMOVE,

      E_LINK_CHANGE,
      E_AUTO_NEGATIOTION
    };

    enum class State {
      E_UP,
      E_DOWN
    };

    enum class Duplex {
      E_DUPLEX_HALF,
      E_DUPLEX_FULL,
      E_DUPLEX_AUTO
    };

    enum class Speed {
      E_SPEED_10M,
      E_SPEED_100M,
      E_SPEED_1G,
      E_SPEED_10G,
      E_SPEED_AUTO
    };

    using Ids = std::set<std::shared_ptr<Port>>;

    using Notify =
      std::function<void(Event, const Ids&)>;

    class Port {
      friend GBoard;
      GBoard *m_board;

      Ids m_to_ports;

      Speed m_speed;
      Duplex m_duplex;
      State m_state;

      unsigned m_logic_port;
      unsigned m_physic_port;

      Port(GBoard*, unsigned, unsigned);

      public:
        operator unsigned() const;

        unsigned GetPhysicPort() const;
        unsigned GetLogicPort() const;

        std::tuple<Speed, Duplex, State> GetStatus() const;

        void SetSpeed(Speed, Duplex);
        void SetAdminMode(State);

        void Add(const Ids&);
        void Sub(const Ids&);
        Ids Get() const;
    };

    inline
    std::ostream& operator<<(std::ostream &os, const State &s) {
      switch (s) {
        case State::E_UP  : return os << "Up";
        case State::E_DOWN: return os << "Down";
      }

      return os;
    }

    inline
    std::ostream& operator<<(std::ostream &os, const Duplex &d) {
      switch (d) {
        case Duplex::E_DUPLEX_HALF: return os << "Half";
        case Duplex::E_DUPLEX_FULL: return os << "Full";
        case Duplex::E_DUPLEX_AUTO: return os << "Auto";
      }

      return os;
    }

    inline
    std::ostream& operator<<(std::ostream &os, const Speed &s) {
      switch (s) {
        case Speed::E_SPEED_10M : return os << "10M";
        case Speed::E_SPEED_100M: return os << "100M";
        case Speed::E_SPEED_1G  : return os << "1G";
        case Speed::E_SPEED_10G : return os << "10G";
        case Speed::E_SPEED_AUTO: return os << "Auto";
      }

      return os;
    }

    inline
    std::ostream& operator<<(std::ostream &os, const Event &e) {
      switch (e) {
        case Event::E_CREATE          : return os << "Create";
        case Event::E_REMOVE          : return os << "Remove";
        case Event::E_LINK_CHANGE     : return os << "Link";
        case Event::E_AUTO_NEGATIOTION: return os << "AutoNeg";
      }

      return os;
    }

    inline
    std::ostream& operator<<(std::ostream &os, const Port &p) {
      auto [speed, duplex, state] = p.GetStatus();
      std::string ports;

      for (auto to : p.Get()) {
        ports += " " + std::to_string(to->GetPhysicPort()) + " ";
      }

      os << "\n ===- Port -===";
      os << "\n  - physic : " << p.GetPhysicPort();
      os << "\n  - logic  : " << p.GetLogicPort();
      os << "\n  - speed  : " << speed;
      os << "\n  - duplex : " << duplex;
      os << "\n  - state  : " << state;

      os << "\n to {" << ports << "}";
      os << "\n\n";

      return os;
    }
  }
}

#endif /* PORT_H__ */
