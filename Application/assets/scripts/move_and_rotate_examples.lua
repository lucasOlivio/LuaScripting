local commands = require("assets.scripts.commands")

local entity = -1

function onstart(thisEntity)
    entity = thisEntity
    print("startn entity: " .. entity)
    
    -- Example usage:
    local moveCommand1 = commands.MoveCommand(entity, {1600.0, 0.0, 0.0}, 6, 0.0, 0.0, false)
    local moveCommand2 = commands.MoveCommand(entity, {1600.0, 1600.0, 0.0}, 6, 0.0, 0.0, false)
    local moveCommand3 = commands.MoveCommand(entity, {0.0, 1600.0, 0.0}, 6, 0.0, 0.0, true)

    
    local orientCommand1 = commands.OrientCommand(entity, {0.0, 90.0, 0.0}, 6, 0.0, 0.0, false)
    local orientCommand2 = commands.OrientCommand(entity, {0.0, 0.0, 90.0}, 6, 0.0, 0.0, false)
    local orientCommand3 = commands.OrientCommand(entity, {0.0, 180.0, -90.0}, 6, 0.0, 0.0, true)

    -- Create the main command group
    local moveGroup = CommandGroup:new("movegroup")
    moveGroup:addCommand(moveCommand1, false) 
    moveGroup:addCommand(moveCommand2, false)
    moveGroup:addCommand(moveCommand3, false)  

    local orientGroup = CommandGroup:new("orientgroup")
    orientGroup:addCommand(orientCommand1, false) 
    orientGroup:addCommand(orientCommand2, false) 
    orientGroup:addCommand(orientCommand3, false) 

    local mainCommandGroup = CommandGroup:new("movegroup")
    mainCommandGroup:addCommand(moveGroup, true) 
    mainCommandGroup:addCommand(orientGroup, true) 

    commands.DispatchCommands(mainCommandGroup)
end

function update(deltatime)
end
