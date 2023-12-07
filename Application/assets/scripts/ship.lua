local gameplay = require("assets.scripts.gameplay")
local inputkeys    = require("assets.scripts.common.inputkeys")
local inputactions = require("assets.scripts.common.inputactions")

function onstart()
    print("startn entity: " .. entity)

end

function onkeyinput(pressedkey, action, mods, scancode)
    if (pressedkey == inputkeys.W and (action == inputactions.PRESS)) then 
        MoveForwardZigZag(entity, {0.0, 0.0, -10000.0}, 250.0, 10, 10.0, 50.0)
    end
end

function oncollision(tagCollided)
    gameplay.DestroyShip(entity)
end
