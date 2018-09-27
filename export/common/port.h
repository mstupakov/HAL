#ifndef PORT_H__
#define PORT_H__

#include "common.h"

#include <list>
#include <tuple>
#include <functional>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>

namespace hal {
  struct PAccessor;

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

    struct MacStats {
      uint64_t rx_good_octets;
      uint64_t rx_bad_octets;

      uint64_t rx_good_pkts;
      uint64_t rx_bad_pkts;
      uint64_t rx_good_bc_pkts;
      uint64_t rx_good_mc_pkts;
      uint64_t rx_good_fc_pkts;
      uint64_t rx_good_uc_pkts;

      uint64_t rx_error_mac_pkts;
      uint64_t rx_undersize_pkts;
      uint64_t rx_oversize_pkts;
      uint64_t rx_jabber_pkts;
      uint64_t rx_fragments_pkts;

      uint64_t total_64_pkts;
      uint64_t total_65_127_pkts;
      uint64_t total_128_255_pkts;
      uint64_t total_256_511_pkts;
      uint64_t total_512_1023_pkts;
      uint64_t total_1024_1518_pkts;
      uint64_t total_1519_max_pkts;
      uint64_t total_drop_pkts;

      uint64_t tx_error_mac_pkts;
      uint64_t tx_collision_mac_pkts;

      uint64_t tx_good_octets;
      uint64_t tx_good_pkts;
      uint64_t tx_good_mc_pkts;
      uint64_t tx_good_bc_pkts;
      uint64_t tx_good_fc_pkts;
      uint64_t tx_good_uc_pkts;
    };

    using Ids = std::set<std::shared_ptr<Port>>;

    using Notify =
      std::function<void(Event, const Ids&)>;

    class Port {
      friend PAccessor;

      GBoard *m_board;

      Ids m_to_ports;
      MacStats m_statistics;

      std::tuple<Speed, Duplex, State> m_cfg_cur;
      std::tuple<Speed, Duplex, State> m_cfg_new;

      std::tuple<Speed, Duplex, State> m_opr_cur;
      std::tuple<Speed, Duplex, State> m_opr_old;

      unsigned m_logic_port;
      unsigned m_physic_port;

      Port(GBoard*, unsigned, unsigned);

      public:
        operator unsigned() const;

        unsigned GetPhysicPort() const;

        std::tuple<Speed, Duplex, State> GetStatus();
        std::tuple<Speed, Duplex, State> GetCfg();

        void SetSpeed(Speed, Duplex);
        void SetAdminMode(State);

        MacStats GetStatistics();
        void Flush();

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
    std::ostream& operator<<(std::ostream &os, Port &p) {
      Speed speed; Duplex duplex; State state;
      std::tie(speed, duplex, state) = p.GetStatus();

      Speed cfg_speed; Duplex cfg_duplex; State cfg_state;
      std::tie(cfg_speed, cfg_duplex, cfg_state) = p.GetCfg();

      std::string ports;

      for (auto to : p.Get()) {
        ports += " " + std::to_string(to->GetPhysicPort()) + " ";
      }

      std::stringstream ss;
      ss << "\n ===- Port -===";
      ss << "\n  - physic : " << p.GetPhysicPort();
      ss << "\n  - logic  : " << static_cast<unsigned>(p);
      ss << "\n  - speed  : " << speed;
      ss << "\n  - duplex : " << duplex;
      ss << "\n  - link   : " << state;
      ss << "\n  - admin  : " << cfg_state;

      ss << "\n to {" << ports << "}";
      ss << "\n\n";

      return os << ss.str();
    }

    inline
    std::ostream& operator<<(std::ostream &os, const MacStats &stats) {
      std::stringstream ss;

      ss << "\n=== Rx:";

      ss << "\n  - Bytes good         : " << stats.rx_good_octets;
      ss << "\n  - Bytes bad          : " << stats.rx_bad_octets;

      ss << "\n  - Frames good        : " << stats.rx_good_pkts;
      ss << "\n  - Frames bad         : " << stats.rx_bad_pkts;

      ss << "\n  - Unicast            : " << stats.rx_good_uc_pkts;
      ss << "\n  - Multicast          : " << stats.rx_good_mc_pkts;
      ss << "\n  - Broadcast          : " << stats.rx_good_bc_pkts;
      ss << "\n  - Flow ctrl          : " << stats.rx_good_fc_pkts;

      ss << "\n  - Mac error          : " << stats.rx_error_mac_pkts;

      ss << "\n  - Undersized         : " << stats.rx_undersize_pkts;
      ss << "\n  - Oversized          : " << stats.rx_oversize_pkts;
      ss << "\n  - Jabber             : " << stats.rx_jabber_pkts;
      ss << "\n  - Fragmets           : " << stats.rx_fragments_pkts;

      ss << "\n\n=== Tx:";

      ss << "\n  - Bytes good         : " << stats.tx_good_octets;
      ss << "\n  - Frames good        : " << stats.tx_good_pkts;

      ss << "\n  - Unicast            : " << stats.tx_good_uc_pkts;
      ss << "\n  - Multicast          : " << stats.tx_good_mc_pkts;
      ss << "\n  - Broadcast          : " << stats.tx_good_bc_pkts;
      ss << "\n  - Flow ctrl          : " << stats.tx_good_fc_pkts;

      ss << "\n  - Mac error          : " << stats.tx_error_mac_pkts;
      ss << "\n  - Mac collision      : " << stats.tx_collision_mac_pkts;

      ss << "\n\n=== Total:";
      ss << "\n  - Octets under 64    : " << stats.total_64_pkts;
      ss << "\n  - Octets over 1518   : " << stats.total_1519_max_pkts;
      ss << "\n  - Dropped            : " << stats.total_drop_pkts;

      ss << "\n  - Octets 65 - 127    : " << stats.total_65_127_pkts;
      ss << "\n  - Octets 128 - 255   : " << stats.total_128_255_pkts;
      ss << "\n  - Octets 256 - 511   : " << stats.total_256_511_pkts;
      ss << "\n  - Octets 512 - 1023  : " << stats.total_512_1023_pkts;
      ss << "\n  - Octets 1024 - 1518 : " << stats.total_1024_1518_pkts;
      ss << "\n\n";

      return os << ss.str();
    }
  }
}

#endif /* PORT_H__ */
