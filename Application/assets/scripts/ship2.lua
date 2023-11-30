local commands = require("assets.scripts.commands")

local entity = -1

function onstart(thisEntity)
    entity = thisEntity
    print("startn entity: " .. entity)
    
    -- Example usage:
    local controlPoints = {
        {1200.0, 1200.0, 0.0},
        {0.0, 2000.0, 0.0},
        {-1200.0, 1200.0, 0.0}
    }
    local curveCommand = commands.FollowCurveCommand(entity, controlPoints, 3.0, 50.0, 0.0, 0.0, true)

    -- Create the main command group
    local followGroup = CommandGroup:new("curve")
    followGroup:addCommand(curveCommand, false)

    local mainCommandGroup = CommandGroup:new("followGroup")
    mainCommandGroup:addCommand(followGroup, true) 

    commands.DispatchCommands(mainCommandGroup)
end

function update(deltatime)
end
