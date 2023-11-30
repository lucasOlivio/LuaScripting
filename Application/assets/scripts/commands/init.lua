local commandgroups = require("assets.scripts.commands.commandgroup")
local commands = require("assets.scripts.commands.commands")
local utils = require("assets.scripts.commands.utils")

return 
{
    commandgroups.CommandGroup,
    MoveCommand = commands.MoveCommand,
    OrientCommand = commands.OrientCommand,
    FollowCurveCommand = commands.FollowCurveCommand,
    FollowObjectCommand = commands.FollowObjectCommand,
    DispatchCommands = utils.DispatchCommands,
}