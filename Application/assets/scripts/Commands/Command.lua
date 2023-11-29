Command = {
    name = "",
    args = {}
}

function Command:new(o, name, args)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    self.name = name
    self.args = args
    return o
end
