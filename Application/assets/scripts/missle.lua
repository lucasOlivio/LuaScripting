local gameplay = require("assets.scripts.gameplay")
local directions = require("assets.scripts.common.directions")


function onstart()
    print("startn entity: " .. entity)
    
    local acceleration = 10000
    local maxSpeed = 7500
    
    gameplay.Accelerate(entity, directions.FORWARD, acceleration, maxSpeed)
end