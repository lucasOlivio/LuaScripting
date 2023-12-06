local commands = require("assets.scripts.commands")

-- Returns the command UUID
function Shoot(entity)
    x, y, z, rx, ry, rz, scale = GetTransform(entity)

    local destroiedship = commands.CreateEntityCommand("missle", {x, y, z}, {rx, ry, rz})

    local mainCommandGroup = CommandGroup:new("shoot")
    mainCommandGroup:addCommand(destroiedship, true) 

    return commands.DispatchCommands(mainCommandGroup)
end

return {
    Shoot = Shoot
}
