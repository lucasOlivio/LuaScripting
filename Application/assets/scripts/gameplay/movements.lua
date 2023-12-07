local commands = require("assets.scripts.commands")
local json = require("assets.scripts.json")

function MoveHalfCircleForwardZ(startPos, endPos, radius)
    local offset = radius * (math.pi * 0.42)

    local x2 = startPos[1] + offset
    local z2 = startPos[3] 

    local x3 = endPos[1] + offset
    local z3 = endPos[3]

    local controlPoints = {startPos, {x2, endPos[2], z2}, {x3, endPos[2], z3}, endPos}

    return controlPoints
end

-- Move forward doing curves to right and left
-- Returns the command UUID
function MoveForwardZigZag(entity, endPoint, radius, curves, time, timestep)
    local mainCommandGroup = CommandGroup:new("followGroup")
    local x, y, z, rx, ry, rz, scale = GetTransform(entity)

    time = time / curves
    local distanceZ = math.abs(endPoint[3] - z)
    local curvesStep = distanceZ / curves

    local controlPoints = {}
    local curveCommand

    for i=1, curves do
        local xI = endPoint[1]
        local yI = endPoint[2]
        local zI = z - curvesStep
        radius = -radius

        controlPoints = MoveHalfCircleForwardZ({x, y, z}, {xI, yI, zI}, radius)
        curveCommand = commands.FollowCurveCommand(entity, controlPoints, time, timestep, 0.0, 0.0, true)
        mainCommandGroup:addCommand(curveCommand, false)

        -- update initial 
        x = xI
        y = yI
        z = zI
    end

    return commands.DispatchCommands(mainCommandGroup)
end

return {
    MoveForwardZigZag = MoveForwardZigZag,
    MoveHalfCircleForwardZ    = MoveHalfCircleForwardZ
}
