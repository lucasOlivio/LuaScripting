local commands     = require("assets.scripts.commands")
local gameplay     = require("assets.scripts.gameplay")
local inputkeys    = require("assets.scripts.common.inputkeys")
local inputactions = require("assets.scripts.common.inputactions")

function onstart()
    print("startn entity: " .. entity)

   gameplay.Follow(entity, "ship", 0.1, 2000, 90, 500)
end

function onkeyinput(pressedkey, action, mods, scancode)
    if (pressedkey == inputkeys.SPACE and (action == inputactions.PRESS)) then -- Space bar Pressed
        gameplay.Shoot(entity)
    end
end

function oncollision(tagCollided)
end
