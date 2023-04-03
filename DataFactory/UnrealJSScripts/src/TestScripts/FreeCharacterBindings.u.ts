import JS_Pawn_Base from "../Classes/JS_Pawn_Base.u";
import { SetAxisBinding, SetAxisBindings, SetKeyBinding } from '../lib/Input';

function main(): Function
{

    // Do stuff
    //DataFactoryBPLibrary.DF_PrintString(null, "Hola from : " + __filename, EDataFactoryLogVerbosity.Warning, true, true, 5.0, 3);

    fc_bindInputs();

    // clean up the mess
    return function ()
    {
        //GWorld.GetPlayerController(0).Possess(new DefaultPawn(GWorld, char.GetActorLocation()));

        //char.DestroyActor();
    };


}


// binds free character input if its possesed by player controller 0
function fc_bindInputs(): void
{
    let fc = GWorld.GetPlayerPawn(0);


    SetKeyBinding(fc, 'L', "LoadFile");

    // Axis name, Function name Bindings
    let AxisBindings = new Map<string, string>();

    AxisBindings.set("MoveForward_InputAxis", "SetMoveForwardAxisValue");
    AxisBindings.set("MoveRight_InputAxis", "SetMoveRightAxisValue");
    AxisBindings.set("MoveUp_InputAxis", "SetMoveUpAxisValue");
    AxisBindings.set("LookRight_InputAxis", "SetLookRightAxisValue");
    AxisBindings.set("LookUp_InputAxis", "SetLookUpAxisValue");


    SetAxisBindings(fc, AxisBindings);

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

    require("./UnrealJS/bootstrap")('FreeCharacterBindings.u');
}
