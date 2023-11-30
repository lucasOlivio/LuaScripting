local Command = require("assets.scripts.commands.command")

-- Move the entity to the referenced location in "time"
-- easyIn and easyOut are optional, to set the proportion of the time that the obj will be accelerating/decelerating
-- stopAtEnd also optional, should the object stop velocity and accelerating at end or keep going?
function MoveCommand(entity, location, time, easyIn, easyOut, stopAtEnd)
    easyIn    = easyIn or 0.2
    easyOut   = easyOut or 0.1
    stopAtEnd =  (stopAtEnd ~= false) -- workaround to have stopatend default as true
    return Command:new("MoveTo", {entity = entity, location = location, 
                                  time = time, easyIn = easyIn, easyOut = easyOut,
                                  stopAtEnd = stopAtEnd})
end

-- Rotate the entity to the referenced orientation in "time"
-- easyIn and easyOut are optional, to set the proportion of the time that the obj will be accelerating/decelerating
-- stopAtEnd also optional, should the object stop velocity and accelerating at end or keep going?
function OrientCommand(entity, orientation, time, easyIn, easyOut, stopAtEnd)
    easyIn    = easyIn or 0.2
    easyOut   = easyOut or 0.1
    stopAtEnd =  (stopAtEnd ~= false) -- workaround to have stopatend default as true
    return Command:new("OrientTo", {entity = entity, orientation = orientation, 
                                    time = time, easyIn = easyIn, easyOut = easyOut,
                                    stopAtEnd = stopAtEnd})
end

-- Move the object trough a bezier curve defined by the list of control points 
-- controlPoints, a list of vec3 to define the bezier
-- timeStep, # of steps to calculate the curve
-- accRatio and deaccRatio are optional, to set the proportion of the time that the obj will be accelerating/decelerating
-- stopAtEnd also optional, should the object stop velocity and accelerating at end or keep going?
function FollowCurveCommand(entity, controlPoints, time, timeStep, accRatio, deaccRatio, stopAtEnd)
    timeStep   = timeStep or 10
    accRatio   = accRatio or 0.2
    deaccRatio = deaccRatio or 0.1
    stopAtEnd  =  (stopAtEnd ~= false) -- workaround to have stopatend default as true
    return Command:new("FollowCurve", {entity = entity, controlPoints = controlPoints, 
                                    time = time, timeStep = timeStep, 
                                    accRatio = accRatio, deaccRatio = deaccRatio,
                                    stopAtEnd = stopAtEnd})
end

-- Follow a target updating the targets position every "timeStep" seconds
-- targetTag, name of target to follow
-- timeStep, Seconds until updating target position
-- maxSpeed, maximum speed to reach
-- followForever, whether it should run forever or just get to target location
-- easyIn and easyOut optional, set distance from target to accelerate/decelerate (in %)
function FollowObjectCommand(entity, targetTag, timeStep, maxSpeed, followForever, easyIn, easyOut)
    easyIn  = easyIn or 0.2
    easyOut = easyOut or 0.1
    return Command:new("FollowObject", {entity = entity, targetTag = targetTag, 
                                    timeStep = timeStep, maxSpeed = maxSpeed, 
                                    easyIn = easyIn, easyOut = easyOut,
                                    followForever = followForever})
end

return {
    MoveCommand = MoveCommand, 
    OrientCommand = OrientCommand, 
    FollowCurveCommand = FollowCurveCommand,
    FollowObjectCommand = FollowObjectCommand
}