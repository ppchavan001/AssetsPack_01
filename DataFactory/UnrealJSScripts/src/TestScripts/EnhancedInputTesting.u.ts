import { DFLOG, DFLOG_ToScreen } from "../lib/Log";

function main(): Function
{

    DFLOG("Hola from 1: " + __filename, EDataFactoryLogVerbosity.Warning, true, true, 5.0, 3);
    let encbk = GWorld.GetAllActorsOfClass(EnhancedInputManager_Backend);
    let act = encbk.OutActors[0];
    if (act instanceof EnhancedInputManager_Backend)
    {
        if (act.AllAvailableInputContexts.length > 0)
        {

            for (let index = 0; index < act.AllAvailableInputContexts.length; ++index)
            {
                let mappingcontext = act.AllAvailableInputContexts[index];

                if (mappingcontext.ContextDescription.includes("FreePawnInputContext"))
                {
                    mappingcontext.UnmapAll();
                    DFLOG("Updating FreePawnInputContext : " + mappingcontext, EDataFactoryLogVerbosity.Warning, true, true, 5.0, 3);

                    const FreePawnMappings = new Map();
                    FreePawnMappings.set('IA_LookRight', ["MouseX"]);
                    FreePawnMappings.set('IA_LookUp', ["MouseY"]);
                    FreePawnMappings.set('IA_MoveForward', ["W", "S"]);
                    FreePawnMappings.set('IA_MoveRight', ["A", "D"]);
                    FreePawnMappings.set('IA_MoveUp', ["Q", "E"]);


                    let ia2 = BFL_EnhancedInputManager.NewUObjectOfClass(mappingcontext, InputAction);

                    if (ia2 instanceof InputAction)
                    {
                        let mods: InputModifier[] = [new InputModifierNegate];
                        let triggers: InputTrigger[] = [];
                        //ia = BFL_EnhancedInputManager.NewInputAction(triggers, mods);

                        let oldmapps = mappingcontext.Mappings;

                        let map1 = new EnhancedActionKeyMapping();
                        map1.Action = ia2;
                        map1.Key = DataFactoryBPLibrary.GetKeyFromName('W');
                        oldmapps.push(map1);

                        let map2 = new EnhancedActionKeyMapping();
                        map2.Action = ia2;
                        map2.Key = DataFactoryBPLibrary.GetKeyFromName('S');
                        map2.Modifiers = mods;
                        oldmapps.push(map2);

                        mappingcontext.Mappings = oldmapps;
                        //DFLOG_ToScreen("Oldmaps :" + oldmapps.length.toString());
                        //DFLOG_ToScreen("NewMaps :" + mappingcontext.Mappings.length.toString());


                        let pc = GWorld.GetPlayerController(0);
                        let eic = pc.GetComponentByClass(EnhancedInputComponent)
                        if (eic != null && eic instanceof EnhancedInputComponent)
                        {

                            BFL_EnhancedInputManager.BindActionOnEnhancedInputComponent(
                                eic,
                                ia2,
                                ETriggerEvent.Triggered,
                                GWorld.GetPlayerPawn(0),
                                "SetMoveForwardAxisValue");
                        }
                        else
                        {
                            DFLOG("EnhancedInputComponent on player controller 0 is invalid!" + eic, EDataFactoryLogVerbosity.Error, true, true, 5.0, 3);
                        }


                    }

                    /** rebuild changes */
                    EnhancedInputLibrary.RequestRebuildControlMappingsUsingContext(mappingcontext, true);


                }
                else
                {
                    /** might not be using free pawn.*/
                    DFLOG("EnhancedInputManager_Backend FreePawnInputContext not found in the contexts list.", EDataFactoryLogVerbosity.Warning, true, true, 5.0, 3);
                }
            }


        }
        else
        {

            DFLOG("EnhancedInputManager_Backend AllAvailableInputContexts.length == 0! Add usable input contexts to the actor.", EDataFactoryLogVerbosity.Error, true, true, 5.0, 3);
        }


    }
    else
    {
        DFLOG("EnhancedInputManager_Backend is not found in the level!", EDataFactoryLogVerbosity.Error, true, true, 5.0, 3);
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
