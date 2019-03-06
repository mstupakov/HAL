#include "iboard.h"
#include "iport.h"

#define SOL_CHECK_ARGUMENTS 1

#include <sol.hpp>

#include <iostream>
#include <exception>
#include <functional>
#include <algorithm>
#include <string>

#include <libgen.h>
#include <unistd.h>

#include <readline/readline.h>
#include <readline/history.h>

using namespace hal;

void Init(std::vector<sol::state> &states) {

  for (auto &lua : states) {
    lua.open_libraries(sol::lib::base, sol::lib::os,
                       sol::lib::io, sol::lib::debug);

    lua.script_file("utility.lua");

    auto ns_hal = lua["hal"].get_or_create<sol::table>();

    ns_hal.new_enum<BoardId, false>("BoardId", {
        { "E_Dummy", BoardId::E_DUMMY },
    });

    ns_hal.new_enum<port::Event, false>("Event", {
        { "E_Create"     , port::Event::E_CREATE           },
        { "E_Remove"     , port::Event::E_REMOVE           },
        { "E_Link_Change", port::Event::E_LINK_CHANGE      },
        { "E_Auto_Neg"   , port::Event::E_AUTO_NEGATIOTION },
    });

    ns_hal.new_enum<port::State, false>("State", {
        { "E_Up"  , port::State::E_UP   },
        { "E_Down", port::State::E_DOWN },
    });

    ns_hal.new_enum<port::Speed, false>("Speed", {
        { "E_10M" , port::Speed::E_SPEED_10M  },
        { "E_100M", port::Speed::E_SPEED_100M },
        { "E_1G"  , port::Speed::E_SPEED_1G   },
        { "E_10G" , port::Speed::E_SPEED_10G  },
        { "E_Auto", port::Speed::E_SPEED_AUTO },
    });

    ns_hal.new_enum<port::Duplex, false>("Duplex", {
        { "E_Half", port::Duplex::E_DUPLEX_HALF },
        { "E_Full", port::Duplex::E_DUPLEX_FULL },
        { "E_Auto", port::Duplex::E_DUPLEX_AUTO },
    });

    lua.script(R"(
        hal.BoardId = U.Const(hal.BoardId)
        hal.Event = U.Const(hal.Event)
        hal.State = U.Const(hal.State)
        hal.Speed = U.Const(hal.Speed)
        hal.Duplex = U.Const(hal.Duplex)
    )");

    ns_hal.new_usertype<IBoard>("IBoard",
        "GetBoard", &IBoard::GetBoard,
        "GetIPort", &IBoard::GetIPort,
        "Init"    , &IBoard::Init
    );

    ns_hal.new_usertype<IPort>("IPort",
        "Subscribe", &IPort::Subscribe,
        "GetPort"  , &IPort::GetPort
    );

    ns_hal.new_usertype<port::Port>("Port",
        "GetPhysicPort", &port::Port::GetPhysicPort,
        "GetStatus"    , &port::Port::GetStatus,
        "GetCfg"       , &port::Port::GetCfg,
        "SetSpeed"     , &port::Port::SetSpeed,
        "SetAdminMode" , &port::Port::SetAdminMode,
        "GetStatistics", &port::Port::GetStatistics,
        "Flush"        , &port::Port::Flush,
        "Add"          , &port::Port::Add,
        "Sub"          , &port::Port::Sub,
        "Get"          , &port::Port::Get
    );

    ns_hal.new_usertype<port::MacStats>("MacStats",
      sol::meta_function::index, [] (port::MacStats s, std::string n) {

          std::map<std::string, std::function<uint64_t()>> map = {
            { "rx", [&s] () { return s.rx_good_octets; } },
            { "tx", [&s] () { return s.tx_good_octets; } },
          };

        return map.at(n)();
    });

    ns_hal.new_usertype<port::Ids>("Ids",
      sol::call_constructor, [] (sol::variadic_args va) {

        port::Ids ids;
        for (std::shared_ptr<port::Port> p : va) {
          ids.insert(p);
        }

        return ids;
      },
      sol::meta_function::index, [] (port::Ids ids, unsigned i) {

        if (ids.size() <= i - 1) {
          throw std::runtime_error("Index is out of range");
        }

        auto it = std::begin(ids);
        std::advance(it, i - 1);

        return *it;
    });

    lua.script_file("init.lua");
    lua.script_file("script.lua");
  }
}

int main(int argc, char *argv[]) {
  chdir(dirname(argv[0]));

  std::vector<sol::state> lua(2);
  Init(lua);

  lua[0]["PassToNextLuaState"] = [&lua] (sol::variadic_args va) {

    std::vector<sol::object> args;

    for (auto obj : va) {
      if (obj.is<IPort>()) {
        args.push_back(
            sol::make_object(lua[1], obj.as<IPort>()));
      } else {
        args.push_back(obj);
      }
    }

    lua[1]["NextLuaState"](sol::as_args(args));
  };

  std::vector<std::string> const args(argv, argv + argc);
  lua[0]["Main"](sol::as_args(args));

  while (true) {
    try {
      std::unique_ptr<char, std::function<void(char*)>>
        line(readline("cmd>> "), [] (char* p) { free(p); });

      if (!line) {
        break;
      }

      if (*line.get()) {
        add_history(line.get());
      }

      lua[0].script(line.get());
    } catch (const std::exception &e) {
      std::cout << e.what() << '\n';
    } catch (...) {
      std::cout << "Unexpected error!\n";
    }
  }

  return 0;
}
