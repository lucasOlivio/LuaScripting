local json = require("assets/scripts/json")

function OnStart()
    print("startn")
    -- Example usage
    -- Create commands
    local moveCommand1 = {name= "MoveTo", args={ entity = 0, finalPos = {-200.0, 0.0, 0.0}, time = 3 }}
    
    -- Create the main command group
    local mainCommandGroup = { serial={ moveCommand1 }, parallel={ }}
    
    
    -- Convert the serialized result to a JSON string
    print("serializing...\n")
    local jsonString = json.stringify(mainCommandGroup, false)
    
    print("sending...")
    SendCommands(jsonString)
 end