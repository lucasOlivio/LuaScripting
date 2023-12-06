local destroyship       = require("assets.scripts.gameplay.destroyship")
local shoot             = require("assets.scripts.gameplay.shoot")
local MoveForwardZigZag = require("assets.scripts.gameplay.moveforwardzigzag")
local follow            = require("assets.scripts.gameplay.follow")
local accelerate        = require("assets.scripts.gameplay.accelerate")

return 
{
    DestroyShip       = destroyship.DestroyShip,
    Shoot             = shoot.Shoot,
    MoveForwardZigZag = MoveForwardZigZag.MoveForwardZigZag,
    Follow            = follow.Follow,
    Accelerate        = accelerate.Accelerate
}