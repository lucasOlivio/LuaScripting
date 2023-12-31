local gameplay = require("assets.scripts.gameplay")
local inputkeys    = require("assets.scripts.common.inputkeys")
local inputactions = require("assets.scripts.common.inputactions")

function onstart()
    print("startn entity: " .. entity)

end

function onkeyinput(pressedkey, action, mods, scancode)
    if (pressedkey == inputkeys.W and (action == inputactions.PRESS)) then 
        MoveForwardZigZag(entity, {50000.0, 50000.0, 10000.0}, 1500.0, 10, 10.0, 100.0)
    end
end

function oncollision(tagCollided)
    gameplay.DestroyShip(entity)
end
