import JS_Pawn_Base from "../Classes/JS_Pawn_Base.u";

function main(): Function
{

    // Do stuff
    //DataFactoryBPLibrary.DF_PrintString(undefined, "Hola from : " + __filename, EDataFactoryLogVerbosity.Warning, true, true, 5.0, 3);

    let fc = GWorld.GetPlayerPawn(0);


    // clean up the mess
    return function ()
    {
        //GWorld.GetPlayerController(0).Possess(new DefaultPawn(GWorld, char.GetActorLocation()));

        //char.DestroyActor();
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

    require("./UnrealJS/bootstrap")('TP_Character_JS.u');
}
