Command = {}
Command.__index = Command

function Command:new(name, args)
    local command = {
        command = {
            name = name,
            args = args
        }
    }
    setmetatable(command, self)
    return command
end

return Command