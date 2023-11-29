local Command = require("assets/scripts/commands/command")

-- Move the entity to the referenced location in "time"
function MoveCommand(entity, location, time)
    return Command:new("MoveTo", {entity = entity, location = location, time = time})
end

return MoveCommand