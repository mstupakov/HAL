U = {}

U.PrintEnv = function (env)
  env = env or _G

  for k,v in pairs(env) do
    print(k, v)
  end
end

U.GC = function ()
  collectgarbage()
end

U.SleepSys = function (s)
  os.execute("sleep " .. tonumber(s))
end

U.Sleep = function (s)
  local ntime = os.time() + s
  repeat until os.time() > ntime
end

U.Do = function (f)
  dofile(f)
end

U.Const = function (table)

  local meta_table = {
    __index = function(self, key)
      if table[key] == nil then
        error("Attempted access a non existant field: " .. key)
      else
        return table[key]
      end
    end,

    __newindex = function(self, key, value)
      error("Attempt to modify const table: " .. key .. " " .. value)
    end,

    __tostring = function()
      s = "\n"

      for k, v in pairs(table) do
        s = s .. "[" .. v .. "] = " .. k .. "\n"
      end

      return s
    end,

    __metatable = false
  }

  return setmetatable({}, meta_table)
end
