CommandGroup = {
    serial = {},
    parallel = {}
}

function CommandGroup:new(o, serial, parallel)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    self.serial = serial or {}
    self.parallel = parallel or {}
    return o
end