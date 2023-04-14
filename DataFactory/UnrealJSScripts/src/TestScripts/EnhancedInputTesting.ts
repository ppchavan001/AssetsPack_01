import { KeyMapType, EI_JS_Mapping } from "../lib/EnhancedInput/EnhancedInputLib";
import IM_DeadZoneAbsolute from "../lib/EnhancedInput/InputModifiers/IM_DeadZoneAbsolute.u";
import { DFLOG, DFLOG_Error, DFLOG_ToConsole, DFLOG_ToScreen } from "../lib/Log";


function main(): Function
{

    DFLOG("JS Executing " + __filename, EDataFactoryLogVerbosity.Warning, true, true, 5.0, 3);

    let pc = GWorld.GetPlayerController(0);
    let FreePawnMappingContext = BFL_EnhancedInputManager.NewUObjectOfClass(pc, InputMappingContext);;
    if (FreePawnMappingContext instanceof InputMappingContext)
    {
        FreePawnMappingContext.ContextDescription = "FreePawnMappingContext generated from JS in : " + __filename;

        //#region Vive Mods Setup

        let ViveTrackpadDeadZone = new IM_DeadZoneAbsolute(FreePawnMappingContext);
        ViveTrackpadDeadZone.LowerThreshold = -0.5;
        ViveTrackpadDeadZone.UpperThreshold = 0.5;

        let ViveTrackpadMods: InputModifier[] = [ViveTrackpadDeadZone];

        //#endregion

        //#region Generate FreePawn Mappings

        let KeyMap = new Map<string, KeyMapType[]>();

        let bSwapLeftRightControllers = false;

        let LeftTrackPadX = (bSwapLeftRightControllers ? 'Vive_Left_Trackpad_X' : 'Vive_Right_Trackpad_X');
        let LeftTrackPadY = (bSwapLeftRightControllers ? 'Vive_Left_Trackpad_Y' : 'Vive_Right_Trackpad_Y');
        let RightTrackPadX = (bSwapLeftRightControllers ? 'Vive_Right_Trackpad_X' : 'Vive_Left_Trackpad_X');
        let RightTrackPadY = (bSwapLeftRightControllers ? 'Vive_Right_Trackpad_Y' : 'Vive_Left_Trackpad_Y');



        KeyMap.set("SetMoveForwardAxisValue",
            [
                new KeyMapType("W"),
                new KeyMapType("S", [], [new InputModifierNegate()]),
                new KeyMapType(LeftTrackPadX, [], ViveTrackpadMods)
            ]);

        KeyMap.set("SetMoveRightAxisValue",
            [
                new KeyMapType("D"),
                new KeyMapType("A", [], [new InputModifierNegate()]),
                new KeyMapType(LeftTrackPadY, [], ViveTrackpadMods),
            ]);


        KeyMap.set("SetMoveUpAxisValue",
            [
                new KeyMapType("E"),
                new KeyMapType("Q", [], [new InputModifierNegate()]),
                new KeyMapType(RightTrackPadY, [], ViveTrackpadMods),
            ]);

        KeyMap.set("SetLookRightAxisValue",
            [
                new KeyMapType("MouseX"),
                //new KeyMapType("A", [], [new InputModifierNegate()]),
                new KeyMapType(RightTrackPadX, [], ViveTrackpadMods),
            ]);

        KeyMap.set("SetLookUpAxisValue",
            [
                new KeyMapType("MouseY"),
                //new KeyMapType("A", [], [new InputModifierNegate()]),
                //new KeyMapType("Vive_Right_Trackpad_X", [], ViveTrackpadMods),
            ]);





        for (const [key, value] of KeyMap.entries())
        {
            let NewMapping = new EI_JS_Mapping(
                FreePawnMappingContext,
                GWorld.GetPlayerPawn(0) /** Expecting the Player pawn to be a FreePawn */,
                key,
                value,
                ETriggerEvent.Triggered,
                true,
                false);

            //MoveForwardMapping.ProcessMappingData();
        }

        //#endregion


        /** rebuild changes */
        EnhancedInputLibrary.RequestRebuildControlMappingsUsingContext(FreePawnMappingContext, true);

        BFL_EnhancedInputManager.AddNewInputMappingContext(pc, FreePawnMappingContext, 0);
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
