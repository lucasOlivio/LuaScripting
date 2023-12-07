local commands = require("assets.scripts.commands")
local directions = require("assets.scripts.common.directions")

-- Returns the command UUID
function DestroyShip(entity)
    x, y, z, rx, ry, rz, scale = GetTransform(entity)

    local selfdestroy = commands.DestroyEntityCommand(entity)
    local destroiedship = commands.CreateEntityCommand(entity, "shipdestroied", {x, y, z}, directions.FORWARD)

    local mainCommandGroup = CommandGroup:new("destroyship")
    mainCommandGroup:addCommand(destroiedship, false) 
    mainCommandGroup:addCommand(selfdestroy, false) 

    return commands.DispatchCommands(mainCommandGroup)
end

return {
    DestroyShip = DestroyShip
}
