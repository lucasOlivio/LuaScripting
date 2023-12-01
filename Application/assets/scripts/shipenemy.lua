local commands = require("assets.scripts.commands")

local entity = "shipenemy"

function onstart()
    print("startn entity: " .. entity)
    
    -- Example usage:
    local followobjCommand = commands.FollowObjectCommand(entity, "ship", 0.1, 500, 50, {0.0, -400.0, 0.0})

    -- Create the main command group
    local followGroup = CommandGroup:new("followobjCommand")
    followGroup:addCommand(followobjCommand, false)

    local mainCommandGroup = CommandGroup:new("followGroup")
    mainCommandGroup:addCommand(followGroup, false) 

    commands.DispatchCommands(mainCommandGroup)
end

function update(deltatime)

    if Action("shoot") then
        x, y, z, rx, ry, rz, scale = GetTransform(entity)

        local shootCommand = commands.CreateEntityCommand("missle", {x, y, z}, {rx, ry, rz})

        local mainCommandGroup = CommandGroup:new("shootgroup")
        mainCommandGroup:addCommand(shootCommand, false) 

        commands.DispatchCommands(mainCommandGroup)
    end
end

function oncollision(tagCollided)
end
