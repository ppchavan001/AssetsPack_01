import { DFLOG } from "../lib/Log";

function main(): Function
{

    // Do stuff


    DFLOG("Hola from 1: " + __filename, EDataFactoryLogVerbosity.Warning, true, true, 5.0, 3);
    let encbk = GWorld.GetAllActorsOfClass(EnhancedInputManager_Backend);
    let ctx;
    let act = encbk.OutActors[0];
    if (act instanceof EnhancedInputManager_Backend)
    {

        let mappingcontext = act.AllAvailableInputContexts[0];
        //mappingcontext = InputMappingContext.CreateDefaultSubobject("InputMappingContext");

        DFLOG("Updating Context : " + mappingcontext, EDataFactoryLogVerbosity.Warning, true, true, 5.0, 3);
        /** new Action setup */
        // let IA_MoveForward = InputAction.CreateDefaultSubobject("IA_MoveForward");
        // IA_MoveForward.bConsumeInput = true;
        // IA_MoveForward.bTriggerWhenPaused = false;
        // IA_MoveForward.bReserveAllMappings = false;
        // IA_MoveForward.ValueType = EInputActionValueType.Axis1D;

        // /** Bind actions to context */
        // let newmapping = new EnhancedActionKeyMapping();
        // newmapping.Key = DataFactoryBPLibrary.GetKeyFromName("W");
        // mappingcontext.Mappings.push(newmapping);

        let ia: InputAction = mappingcontext.Mappings[0].Action;

        /** For some reason cant assign FKey to FKey */
        //mappingcontext.Mappings[0].Key = DataFactoryBPLibrary.GetKeyFromName('W');
        mappingcontext.MapKey(ia, DataFactoryBPLibrary.GetKeyFromName('W'));
        //mappingcontext.UnmapKey(ia,);

        /** rebuild changes */
        EnhancedInputLibrary.RequestRebuildControlMappingsUsingContext(mappingcontext, true);

        /** Bind functions to actions */

        let pc = GWorld.GetPlayerController(0);
        let enccomp = pc.GetComponentByClass(EnhancedInputComponent)


        if (enccomp != null && enccomp instanceof EnhancedInputComponent)
        {

            BFL_EnhancedInputManager.BindActionOnEnhancedInputComponent(
                enccomp,
                ia,
                ETriggerEvent.Triggered,
                GWorld.GetPlayerPawn(0),
                "SetLookUpAxisValue");
        }
        else
        {
            DFLOG("enccomp is invalid!" + enccomp, EDataFactoryLogVerbosity.Error, true, true, 5.0, 3);
        }

        BFL_EnhancedInputManager.AddNewInputMappingContext(mappingcontext, mappingcontext, 0);

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

    require("./UnrealJS/bootstrap")('./TestScripts/EnhancedInputTesting.u');
}
