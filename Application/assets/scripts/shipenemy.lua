local commands     = require("assets.scripts.commands")
local gameplay     = require("assets.scripts.gameplay")
local inputkeys    = require("assets.scripts.common.inputkeys")
local inputactions = require("assets.scripts.common.inputactions")

function onstart()
    print("startn entity: " .. entity)

end

function onkeyinput(pressedkey, action, mods, scancode)
    if (pressedkey == inputkeys.SPACE and (action == inputactions.PRESS)) then
        gameplay.Shoot(entity)
    end

    
    if (pressedkey == inputkeys.W and (action == inputactions.PRESS)) then 
        gameplay.Follow(entity, "ship", 0.1, 2500, 0.5, 1500, {0.0, 0.0, 0.0})
    end
end

function oncollision(tagCollided)
end
