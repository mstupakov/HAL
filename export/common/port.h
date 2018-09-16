#ifndef PORT_H__
#define PORT_H__

#include <list>
#include <functional>
#include <iostream>
#include <sstream>

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

    using Ids = std::list<std::shared_ptr<Port>>;

    using Notify =
      std::function<void(const Event, const Ids&)>;

    class Port {
      Speed m_speed;
      Duplex m_duplex;
      State m_state;

      unsigned m_logic_port;
      unsigned m_physic_port;

      public:
        Port() {
          static int i = 1;
          m_physic_port = i++;
        }
        operator unsigned() { return m_physic_port; };

        unsigned GetPhysicPort() {
          return m_physic_port;
        }

        unsigned GetLogicPort() {
          return m_logic_port;
        }

        std::tuple<Speed, Duplex, State> GetStatus() {
          return std::make_tuple(Speed::E_SPEED_100M, Duplex::E_DUPLEX_FULL, State::E_UP);
        }

        void SetSpeed(Speed speed, Duplex duplex) {
          m_speed = speed;
          m_duplex = duplex;
        }

        void SetAdminMode(State state) {
          m_state = state;
        }

        void Add(const Ids& ports) {
          for (auto p : ports) {
            std::cerr << __PRETTY_FUNCTION__ << ": " << p->m_physic_port << '\n';
          }
        }

        void Sub(const Ids& ports) {
          for (auto p : ports) {
            std::cerr << __PRETTY_FUNCTION__ << ": " << p->m_physic_port << '\n';
          }
        }

        Ids Get() {

        }
    };
  }
}

#endif /* PORT_H__ */
