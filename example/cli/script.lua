-- Shortcuts
E_DUMMY = hal.BoardId.E_Dummy

E_CREATE      = hal.Event.E_Create
E_REMOVE      = hal.Event.E_Remove
E_LINK_CHANGE = hal.Event.E_Link_Change
E_AUTO_NEG    = hal.Event.E_Auto_Neg

E_UP   = hal.State.E_Up
E_DOWN = hal.State.E_Down

E_DUPLEX_AUTO = hal.Duplex.E_Auto
E_DUPLEX_FULL = hal.Duplex.E_Full
E_DUPLEX_HALF = hal.Duplex.E_Half

E_SPEED_AUTO = hal.Speed.E_Auto
E_SPEED_10M  = hal.Speed.E_10M
E_SPEED_100M = hal.Speed.E_100M
E_SPEED_1G   = hal.Speed.E_1G


PortNotify = function (event, ids)
  print('========== Interrupt ===========')
  print('Event: ' .. event, ids)

  for i = 1, #ids do
    print(ids[i]:GetStatistics())
  end

  print('================================')
end


NextLuaState = function (iport, p)
  iport:Subscribe(PortNotify)
end


Test = function (platform)

  platform = platform or E_DUMMY

  -- Initialization procedure
  board = hal.IBoard.GetBoard(platform)
  iport = board:GetIPort()

  PassToNextLuaState(iport)
  board:Init()

  -- Getting ports instance
  p0 = iport:GetPort(0)
  p1 = iport:GetPort(1)
  p2 = iport:GetPort(2)
  p3 = iport:GetPort(3)

  -- Experiments
  p2:Flush()

  print(p0:Get())

  print(p0)
  print(p0:SetAdminMode(E_DOWN))
  print(p0:SetSpeed(E_SPEED_1G, E_DUPLEX_AUTO))
  print(p0)
  print(p0:GetStatistics())

  print("rx: " .. p0:GetStatistics()["rx"])
  print("tx: " .. p0:GetStatistics()["tx"])

  -- Iterate through all ports and
  -- gettting / setting parameters
  for k, v in pairs(iport) do
    print(v)

    v:GetPhysicPort()
    v:GetStatus()
    v:GetCfg()

    v:SetSpeed(E_SPEED_100M, E_DUPLEX_AUTO)
    v:SetAdminMode(E_UP)

    v:GetStatistics()

    v:Flush()

    v:Add(hal.Ids(p0, p1, p2, p3))
    v:Sub(hal.Ids(p1))

    v:Get()
  end

  -- Print all ports object
  U.PrintEnv(iport)
end


-- Entry point
Main = function ()
  Help()
  Test()
end
