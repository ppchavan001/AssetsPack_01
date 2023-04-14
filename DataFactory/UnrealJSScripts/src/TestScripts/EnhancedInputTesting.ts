import { KeyMapType, MappingData } from "../lib/EnhancedInputLib";
import { DFLOG, DFLOG_Error, DFLOG_ToConsole, DFLOG_ToScreen } from "../lib/Log";



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
            let ViveTrackpadDeadZone = new InputModifierDeadZone();

            ViveTrackpadDeadZone.LowerThreshold = 0.1;
            ViveTrackpadDeadZone.UpperThreshold = 0.9;

            let ViveTrackpadMods: InputModifier[] = [ViveTrackpadDeadZone];

            let MoveForwardMapping = new MappingData(
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
                false);

            MoveForwardMapping.ProcessMappingData();



            // let oldmapps = mappingcontext.Mappings;

            // let map1 = new EnhancedActionKeyMapping();
            // map1.Action = ia2;
            // map1.Key = DataFactoryBPLibrary.GetKeyFromName('W');
            // oldmapps.push(map1);

            // let map2 = new EnhancedActionKeyMapping();
            // map2.Action = ia2;
            // map2.Key = DataFactoryBPLibrary.GetKeyFromName('Vive_Left_Trackpad_Y');
            // map2.Modifiers = ViveTrackpadMods;
            // oldmapps.push(map2);

            // let map3 = new EnhancedActionKeyMapping();
            // map3.Action = ia2;
            // map3.Key = DataFactoryBPLibrary.GetKeyFromName('Vive_Right_Trackpad_Y');
            // oldmapps.push(map3);


            // let map4 = new EnhancedActionKeyMapping();
            // map4.Action = ia2;
            // map4.Key = DataFactoryBPLibrary.GetKeyFromName('S');
            // map4.Modifiers = [new InputModifierNegate];
            // oldmapps.push(map4);

            // mappingcontext.Mappings = oldmapps;


            // let eic = pc.GetComponentByClass(EnhancedInputComponent)
            // if (eic != null && eic instanceof EnhancedInputComponent)
            // {

            //     BFL_EnhancedInputManager.BindActionOnEnhancedInputComponent(
            //         eic,
            //         ia2,
            //         ETriggerEvent.Triggered,
            //         GWorld.GetPlayerPawn(0),
            //         "SetMoveForwardAxisValue");
            // }
            // else
            // {
            //     DFLOG("EnhancedInputComponent on player controller 0 is invalid!" + eic, EDataFactoryLogVerbosity.Error, true, true, 5.0, 3);
            // }


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
