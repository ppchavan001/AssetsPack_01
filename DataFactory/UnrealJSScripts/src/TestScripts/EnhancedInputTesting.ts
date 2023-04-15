import { KeyMapType, EI_JS_Mapping } from "../lib/EnhancedInput/EnhancedInputLib";
import IM_DeadZoneAbsolute from "../lib/EnhancedInput/InputModifiers/IM_DeadZoneAbsolute.u";
import { DFLOG, DFLOG_Error, DFLOG_ToConsole, DFLOG_ToScreen } from "../lib/Log";


function main(): Function
{

    DFLOG("JS Executing " + __filename, EDataFactoryLogVerbosity.Warning, true, true, 5.0, 3);

    let pc = GWorld.GetPlayerController(0);
    let FreePawnMappingContext = new InputMappingContext; //BFL_EnhancedInputManager.NewUObjectOfClass(pc, InputMappingContext);;

    FreePawnMappingContext.ContextDescription = "FreePawnMappingContext generated from JS in : " + __filename;

    //#region Vive Mods Setup

    let ViveTrackpadDeadZone = new IM_DeadZoneAbsolute(FreePawnMappingContext);
    ViveTrackpadDeadZone.LowerThreshold = -0.5;
    ViveTrackpadDeadZone.UpperThreshold = 0.5;

    let ViveTrackpadMods: InputModifier[] = [ViveTrackpadDeadZone];

    //#endregion

    //#region Generate FreePawn Mappings

    let TriggeredKeyMap = new Map<string, KeyMapType[]>();
    let ToggledKeyMap = new Map<string, KeyMapType[]>();


    let bSwapLeftRightControllers = true;

    let LeftTrackPadX = (bSwapLeftRightControllers ? 'Vive_Left_Trackpad_X' : 'Vive_Right_Trackpad_X');
    let LeftTrackPadY = (bSwapLeftRightControllers ? 'Vive_Left_Trackpad_Y' : 'Vive_Right_Trackpad_Y');
    let RightTrackPadX = (bSwapLeftRightControllers ? 'Vive_Right_Trackpad_X' : 'Vive_Left_Trackpad_X');
    let RightTrackPadY = (bSwapLeftRightControllers ? 'Vive_Right_Trackpad_Y' : 'Vive_Left_Trackpad_Y');

    let LeftTrackPadClick = (bSwapLeftRightControllers ? 'Vive_Left_Trackpad_Click' : 'Vive_Right_Trackpad_Click');
    let RightTrackPadClick = (bSwapLeftRightControllers ? 'Vive_Right_Trackpad_Click' : 'Vive_Left_Trackpad_Click');

    //#region  Movement
    TriggeredKeyMap.set("SetMoveForwardAxisValue",
        [
            new KeyMapType("W"),
            new KeyMapType("S", [], [new InputModifierNegate()]),
            new KeyMapType(LeftTrackPadY, [], ViveTrackpadMods)
        ]);

    TriggeredKeyMap.set("SetMoveRightAxisValue",
        [
            new KeyMapType("D"),
            new KeyMapType("A", [], [new InputModifierNegate()]),
            new KeyMapType(LeftTrackPadX, [], ViveTrackpadMods),
        ]);


    TriggeredKeyMap.set("SetMoveUpAxisValue",
        [
            new KeyMapType("E"),
            new KeyMapType("Q", [], [new InputModifierNegate()]),
            new KeyMapType(RightTrackPadY, [], ViveTrackpadMods),
        ]);
    //#endregion

    //#region Rotation

    TriggeredKeyMap.set("SetLookRightAxisValue",
        [
            new KeyMapType("MouseX"),
            //new KeyMapType("A", [], [new InputModifierNegate()]),
            new KeyMapType(RightTrackPadX, [], ViveTrackpadMods),
        ]);

    TriggeredKeyMap.set("SetLookUpAxisValue",
        [
            new KeyMapType("MouseY"),
            //new KeyMapType("A", [], [new InputModifierNegate()]),
            //new KeyMapType("Vive_Right_Trackpad_X", [], ViveTrackpadMods),
        ]);

    //#endregion

    //#region Toggle Movement & Rotation

    ToggledKeyMap.set("SetbTranslateOnAxisChange",
        [
            //new KeyMapType("MouseY"),
            new KeyMapType("C", [], []),
            new KeyMapType(LeftTrackPadClick, [], []),
            new KeyMapType(RightTrackPadClick, [], [])

        ]);

    ToggledKeyMap.set("SetbRotateOnAxisChange",
        [
            //new KeyMapType("MouseY"),
            //new KeyMapType("A", [], [new InputModifierNegate()]),
            new KeyMapType(RightTrackPadClick, [], [])
        ]);


    //////////////////////////////////////////////////////////////////
    /** TODO : Bind same actions to a @UCLASS in TS to Play Haptics */
    //////////////////////////////////////////////////////////////////


    //#endregion

    for (const [KeyName, KeyMaps] of TriggeredKeyMap.entries())
    {

        let NewMapping = new EI_JS_Mapping(
            FreePawnMappingContext,
            GWorld.GetPlayerPawn(0) /** Expecting the Player pawn to be a FreePawn */,
            KeyName,
            KeyMaps,
            ETriggerEvent.Triggered,
            true,
            false
        );

        //MoveForwardMapping.ProcessMappingData();
    }

    for (const [KeyName, KeyMaps] of ToggledKeyMap.entries())
    {


        let NewMapping = new EI_JS_Mapping(
            FreePawnMappingContext,
            GWorld.GetPlayerPawn(0) /** Expecting the Player pawn to be a FreePawn */,
            KeyName,
            KeyMaps,
            ETriggerEvent.Started,
            true,
            false
        );


        let NewMapping2 = new EI_JS_Mapping(
            FreePawnMappingContext,
            GWorld.GetPlayerPawn(0) /** Expecting the Player pawn to be a FreePawn */,
            KeyName,
            KeyMaps,
            ETriggerEvent.Completed,
            true,
            false
        );

        //MoveForwardMapping.ProcessMappingData();
    }

    //#endregion


    //#region  debug
    // let mdeb = FreePawnMappingContext.Mappings;

    // for (const map of mdeb)
    // {
    //     if (map.Modifiers.length > 1 || map.Action.Modifiers.length > 1)
    //     {
    //         console.log(map);

    //     }
    //     for (const iterator of map.Modifiers)
    //     {
    //         if (iterator instanceof InputModifierScalar)
    //         {
    //             console.log(map);
    //         }
    //     }

    //     for (const iterator of map.Action.Modifiers)
    //     {
    //         if (iterator instanceof InputModifierScalar)
    //         {
    //             console.log(map);
    //         }
    //     }



    // }
    //#endregion



    /** rebuild changes */
    EnhancedInputLibrary.RequestRebuildControlMappingsUsingContext(FreePawnMappingContext, true);
    BFL_EnhancedInputManager.AddNewInputMappingContext(pc, FreePawnMappingContext, 0);


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

    require("./UnrealJS/bootstrap")('EnhancedInputTesting');
}
