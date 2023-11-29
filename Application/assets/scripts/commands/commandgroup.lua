CommandGroup = {}
CommandGroup.__index = CommandGroup

function CommandGroup:new(name)
    local commandGroup = {
        name = name,
        serial = {},
        parallel = {}
    }
    setmetatable(commandGroup, self)
    return commandGroup
end

-- Add a new entry to the batch. 
-- true if command should run in parallel with others or false if should run in serial
function CommandGroup:addCommand(command, isParallel)
    if isParallel then
        table.insert(self.parallel, command)
    else
        table.insert(self.serial, command)
    end
end

return CommandGroup