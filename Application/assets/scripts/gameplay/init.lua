local destroyship = require("assets.scripts.gameplay.destroyship")
local shoot       = require("assets.scripts.gameplay.shoot")
local movements   = require("assets.scripts.gameplay.movements")
local follow      = require("assets.scripts.gameplay.follow")
local accelerate  = require("assets.scripts.gameplay.accelerate")

return 
{
    DestroyShip       = destroyship.DestroyShip,
    Shoot             = shoot.Shoot,
    MoveForwardZigZag = movements.MoveForwardZigZag,
    MoveHalfCircleForwardZ = movements.MoveHalfCircleForwardZ,
    Follow            = follow.Follow,
    Accelerate        = accelerate.Accelerate
}