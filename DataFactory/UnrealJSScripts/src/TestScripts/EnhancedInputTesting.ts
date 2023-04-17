import FreePawnInputMappings from "../lib/EnhancedInput/FreePawnInputMappings";
import { DFLOG, DFLOG_Error, DFLOG_ToConsole, DFLOG_ToScreen } from "../lib/Log";


function main(): Function
{

    DFLOG("JS Executing " + __filename, EDataFactoryLogVerbosity.Warning, true, true, 5.0, 3);
    FreePawnInputMappings.AddFreePawnInputMappingsToPlayerController();
    // clean up the mess
    return function ()
    {
    };
}



// bootstrap to initiate live-reloading dev env.

try
{
    module.exports = () =>
    {
        let cleanup: Function;

        // wait for map to be loaded.
        process.nextTick(() => (cleanup = main()));

        // live-reloadable function should return its cleanup function
        return () => cleanup();
    };
} catch (e)
{

    require("./UnrealJS/bootstrap")('./TestScripts/EnhancedInputTesting');
}
