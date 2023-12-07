local commands = require("assets.scripts.commands")
local directions = require("assets.scripts.common.directions")

-- Returns the command UUID
function Shoot(entity)
    x, y, z, rx, ry, rz, scale = GetTransform(entity)

    local destroiedship = commands.CreateEntityCommand(entity, "missle", {x, y, z}, directions.FORWARD)

    local mainCommandGroup = CommandGroup:new("shoot")
    mainCommandGroup:addCommand(destroiedship, true) 

    return commands.DispatchCommands(mainCommandGroup)
end

return {
    Shoot = Shoot
}
