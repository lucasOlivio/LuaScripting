local Command = require("assets/scripts/commands/command")

-- Move the entity to the referenced location in "time"
-- accRatio and deaccRatio are optional, to set the proportion of the time that the obj will be accelerating/decelerating
-- stopAtEnd also optional, should the object stop velocity and accelerating at end or keep going?
function MoveCommand(entity, location, time, accRatio, deaccRatio, stopAtEnd)
    accRatio = accRatio or 0.2
    deaccRatio = deaccRatio or 0.1
    stopAtEnd =  (stopAtEnd ~= false) -- workaround to have stopatend default as true
    return Command:new("MoveTo", {entity = entity, location = location, 
                                  time = time, accRatio = accRatio, deaccRatio = deaccRatio,
                                  stopAtEnd = stopAtEnd})
end

return MoveCommand