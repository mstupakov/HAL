function Help()
  print([[

Usage:
  Help() - This help

API:
  namespace hal {
    IBoard.GetBoard(BoardId)
    IBoard:Init()
    IBoard:GetIPort()

    IPort:Subscribe(function)
    IPort:GetPort(number)

    Port:GetPhysicPort()
    Port:GetStatus()
    Port:GetCfg()
    Port:SetSpeed(Speed, Duplex)
    Port:SetAdminMode(State)
    Port:GetStatistics()
    Port:Flush()
    Port:Add(Ids)
    Port:Sub(Ids)
    Port:Get()
  }

TYPES:]])

  print("\n  BoardId:")
  print(hal.BoardId)

  print("\n  Event:")
  print(hal.Event)

  print("\n  State:")
  print(hal.State)

  print("\n  Speed:")
  print(hal.Speed)

  print("\n  Duplex:")
  print(hal.Duplex)
end
