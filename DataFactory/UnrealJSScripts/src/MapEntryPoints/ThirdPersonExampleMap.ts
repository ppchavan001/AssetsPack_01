import { DFLOG, DFLOG_ToConsole } from "../lib/Log";

function main(): Function
{

    // Do stuff


    DFLOG_ToConsole("JS Executing : " + __filename);

    Context.RunFile("./TestScripts/EnhancedInputTesting.js");

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

    require("./UnrealJS/bootstrap")('./MapEntryPoints/ThirdPersonExampleMap');
}
