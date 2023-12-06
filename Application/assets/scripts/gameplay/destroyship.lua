local commands = require("assets.scripts.commands")

-- Returns the command UUID
function DestroyShip(entity)
    x, y, z, rx, ry, rz, scale = GetTransform(entity)

    local selfdestroy = commands.DestroyEntityCommand(entity)
    local destroiedship = commands.CreateEntityCommand("shipdestroied", {x, y, z}, {rx, ry, rz})

    local mainCommandGroup = CommandGroup:new("destroyship")
    mainCommandGroup:addCommand(selfdestroy, true) 
    mainCommandGroup:addCommand(destroiedship, true) 

    return commands.DispatchCommands(mainCommandGroup)
end

return {
    DestroyShip = DestroyShip
}
