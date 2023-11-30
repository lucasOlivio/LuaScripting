local commands = require("assets.scripts.commands")

local entity = -1

function onstart(thisEntity)
    entity = thisEntity
    print("startn entity: " .. entity)
    
    -- Example usage:
    local controlPoints = {
        {1600.0, 400.0, 0.0},
        {800.0, 800.0, 0.0},
        {400.0, 1200.0, 0.0},
        {0.0, 1600.0, 0.0}
    }
    local curveCommand = commands.FollowCurveCommand(entity, controlPoints, 10.0, 0.5, 0.0, 0.0, true)

    -- Create the main command group
    local followGroup = CommandGroup:new("curve")
    followGroup:addCommand(curveCommand, false)

    local mainCommandGroup = CommandGroup:new("followGroup")
    mainCommandGroup:addCommand(followGroup, true) 

    commands.DispatchCommands(mainCommandGroup)
end

function update(deltatime)
end
