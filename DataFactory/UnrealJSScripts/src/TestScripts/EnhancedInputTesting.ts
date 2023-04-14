import { KeyMapType, EI_JS_Mapping } from "../lib/EnhancedInput/EnhancedInputLib";
import IM_DeadZoneAbsolute from "../lib/EnhancedInput/InputModifiers/IM_DeadZoneAbsolute.u";
import { DFLOG, DFLOG_Error, DFLOG_ToConsole, DFLOG_ToScreen } from "../lib/Log";

//import { IM_DeadZoneAbsolute_COMPILED } from "../lib/EnhancedInput/InputModifiers/IM_DeadZoneAbsolute.u";


function main(): Function
{

    DFLOG("JS Executing " + __filename, EDataFactoryLogVerbosity.Warning, true, true, 5.0, 3);

    let pc = GWorld.GetPlayerController(0);
    let mappingcontext = BFL_EnhancedInputManager.NewUObjectOfClass(pc, InputMappingContext);;

    if (mappingcontext instanceof InputMappingContext)
    {
        let ia2 = BFL_EnhancedInputManager.NewUObjectOfClass(mappingcontext, InputAction);

        if (ia2 instanceof InputAction)
        {
            let ViveTrackpadDeadZone = new IM_DeadZoneAbsolute(mappingcontext);
            ViveTrackpadDeadZone.LowerThreshold = -0.5;
            ViveTrackpadDeadZone.UpperThreshold = 0.5;

            let ViveTrackpadMods: InputModifier[] = [ViveTrackpadDeadZone];

            let MoveForwardMapping = new EI_JS_Mapping(
                mappingcontext,
                GWorld.GetPlayerPawn(0) /** Player pawn should be a FreePawn */,
                "SetMoveForwardAxisValue",
                ia2,
                [
                    new KeyMapType("W"),
                    new KeyMapType("S", [], [new InputModifierNegate()]),
                    new KeyMapType("Vive_Left_Trackpad_Y", [], ViveTrackpadMods),
                    new KeyMapType("Vive_Right_Trackpad_Y", [], ViveTrackpadMods)
                ],
                ETriggerEvent.Triggered,
                false,
                false);

            MoveForwardMapping.ProcessMappingData();

        }

        /** rebuild changes */
        EnhancedInputLibrary.RequestRebuildControlMappingsUsingContext(mappingcontext, true);

        BFL_EnhancedInputManager.AddNewInputMappingContext(pc, mappingcontext, 0);
    }



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
