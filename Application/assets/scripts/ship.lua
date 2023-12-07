local gameplay = require("assets.scripts.gameplay")

function onstart()
    MoveForwardZigZag(entity, {0.0, 0.0, -10000.0}, 1000.0, 10, 10.0, 50.0)
end

function oncollision(tagCollided)
    gameplay.DestroyShip(entity)
end
