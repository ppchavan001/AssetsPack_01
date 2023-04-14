import { DFLOG } from "../lib/Log";

function main(): Function
{

    // Do stuff


    DFLOG("JS Executing : " + __filename, EDataFactoryLogVerbosity.Warning, true, true, 5.0, 3);


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

    require("./UnrealJS/bootstrap")('./TestScripts/TemplateScript.u');
}
