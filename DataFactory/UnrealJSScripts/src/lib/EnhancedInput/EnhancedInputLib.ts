import { DFLOG, DFLOG_Error, DFLOG_ToConsole, DFLOG_ToScreen } from "../Log";

/**
 * Just a Data structure to hold data for EI_JS_Mapping
 */
export class KeyMapType
{
    /***
     * Holds key
     * information and triggers/ modifiers for the associated key
     * (Check EKeys:: in UnrealEngine for all keys)[A, J, NumPadZero, LeftMouseButton]
     * 
     * Key : Triggers, Mods 
     * can be different depending on the contexts
     */
    constructor(
        readonly KeyName: string,
        readonly InputTriggers: InputTrigger[] = [],
        readonly InputModifiers: InputModifier[] = [],
    )
    { }

}
class KeyMapType_COMPILED extends KeyMapType { }


/** 
 * Enhanced Input JS Mapping object
 * Helps with action, key bindings
 */
export class EI_JS_Mapping
{
    /**
     * Maps the "_FunctionName" from the "_FunctionObject" to the InputAction with given KeyMaps
     * 
     * 
     * _FunctionObject Object that contains the following function. 
     * _FunctionName = Function name on target object. Needs to be UFUNCTION
     * if "bAddMappingDataToContextOnConstruct" is true will call ProcessMappingData() from construct
     * if false will not call ProcessMappingData() at all, needs to be executed by caller.
     *
     * */
    constructor(
        private readonly MappingContext: InputMappingContext,
        private readonly FunctionObject: Object,
        private readonly FunctionName: string,
        private readonly ia: InputAction,
        private readonly KeyMaps: KeyMapType[],
        private readonly TriggerEvent: ETriggerEvent = ETriggerEvent.Triggered,
        private readonly bAddMappingDataToContextOnConstruct: boolean = true,
        private readonly bRebuildMappingsOnContext: boolean = true
    )
    {

        if (bAddMappingDataToContextOnConstruct)
        {
            this.ProcessMappingData();
        }
    }

    private AllKeysInThisObj: string = "";
    public GetAllKeysFromKeyMap(): string
    {

        if (this.AllKeysInThisObj.length > 0)
        {
            /** 
             * Keys have been generated previously 
             * returned saved keys.
            */
            return this.AllKeysInThisObj;
        }

        for (const KeyMap of this.KeyMaps)
        {
            this.AllKeysInThisObj += KeyMap.KeyName + ", ";
        }

        if (this.AllKeysInThisObj.length > 2)
        {
            this.AllKeysInThisObj = this.AllKeysInThisObj.substring(0, this.AllKeysInThisObj.length - 2); // remove last ', '
        }


        return this.AllKeysInThisObj;
    }

    public ProcessMappingData(): void
    {

        /**
         * Build new EnhancedActionKeyMapping for provided keys
         */
        let oldmapps = this.MappingContext.Mappings;
        for (const KeyMap of this.KeyMaps)
        {
            let NewKeyMap = new EnhancedActionKeyMapping();
            NewKeyMap.Action = this.ia;
            NewKeyMap.Key = DataFactoryBPLibrary.GetKeyFromName(KeyMap.KeyName);
            NewKeyMap.Triggers = KeyMap.InputTriggers;
            NewKeyMap.Modifiers = KeyMap.InputModifiers;
            oldmapps.push(NewKeyMap);

        }
        this.MappingContext.Mappings = oldmapps;
        DFLOG_ToConsole("Added new action mapping for keys : [" + this.GetAllKeysFromKeyMap() + "] To Context : " + this.MappingContext.GetName());


        /** 
         * Bind mappings to the function
         *  
         **/
        let pc = GWorld.GetPlayerController(0);
        let eic = pc.GetComponentByClass(EnhancedInputComponent)
        if (eic != null && eic instanceof EnhancedInputComponent)
        {
            if (this.FunctionObject instanceof UObject)
            {
                DFLOG_ToConsole("Binding keys [" + this.GetAllKeysFromKeyMap() + "] to function : " + this.FunctionName);
                BFL_EnhancedInputManager.BindActionOnEnhancedInputComponent(
                    eic,
                    this.ia,
                    this.TriggerEvent,
                    this.FunctionObject,
                    this.FunctionName);


            }
            else
            {
                DFLOG_Error("this._FunctionObject is not a UObject!");

            }
        }
        else
        {
            DFLOG_Error("EnhancedInputComponent on player controller 0 is invalid!" + eic);
        }


        /**
         * Inform context about the mapping changes,
         * only if, this is not handled by the caller 
         */
        if (this.bRebuildMappingsOnContext)
        {
            /** rebuild changes */
            EnhancedInputLibrary.RequestRebuildControlMappingsUsingContext(this.MappingContext, true);
        }

    }

}

class MappingData_COMPILED extends EI_JS_Mapping { };

