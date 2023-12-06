local commands = require("assets.scripts.commands")

-- Move for "distance" in zigzag
-- Returns the command UUID
function MoveForwardZigZag(entity, distance)
    local mainCommandGroup = CommandGroup:new("followGroup")
    x, y, z, rx, ry, rz, scale = GetTransform(entity)

    curves = distance / 5
    local controlPoints = {}
    -- Generate control points for the zigzag path
    for i = 0, curves do
        local xOffset = i % 2 == 0 and 600.0 or -600.0
        local zOffset = i * 300.0
        table.insert(controlPoints, {x + xOffset, y, z - zOffset})

        -- Check if we have 4 control points, then create a FollowCurveCommand
        if #controlPoints == 4 or i == curves then
            local curveCommand = commands.FollowCurveCommand(entity, controlPoints, 7.0, 50.0, 0.0, 0.0, true)
            mainCommandGroup:addCommand(curveCommand, false)

            -- Clear the control points for the next set
            controlPoints = {}
        end
    end

    return commands.DispatchCommands(mainCommandGroup)
end

return {
    MoveForwardZigZag = MoveForwardZigZag
}
