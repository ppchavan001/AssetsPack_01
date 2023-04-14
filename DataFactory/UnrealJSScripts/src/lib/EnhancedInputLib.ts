import { DFLOG, DFLOG_Error, DFLOG_ToConsole, DFLOG_ToScreen } from "../lib/Log";


export class KeyMapType
{
    constructor(
        readonly KeyName: string,
        readonly InputTriggers: InputTrigger[] = [],
        readonly InputModifiers: InputModifier[] = [],
    )
    { }

}
class KeyMapType_COMPILED extends KeyMapType { }


export class MappingData
{
    /** Object that contains the following function. 
    _FunctionObject: Object;
     Function name on target object. Need to be UFUNCTION 
    _FunctionName: string = "";
    
    * if true will call AddMappingDataToContext() from construct
    * if false will not call AddMappingDataToContext() from construct
        readonly bAddMappingDataToContextOnConstruct: boolean = true
    * 

    TriggerEvent: ETriggerEvent = ETriggerEvent.Triggered 

    */
    constructor(
        private readonly MappingContext: InputMappingContext,
        private readonly FunctionObject: Object,
        private readonly FunctionName: string,
        private readonly ia: InputAction,
        private readonly KeyMaps: KeyMapType[],
        private readonly TriggerEvent: ETriggerEvent = ETriggerEvent.Triggered,
        private readonly bAddMappingDataToContextOnConstruct: boolean = true

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


        DFLOG_ToConsole("Adding new action mapping for key : [" + this.GetAllKeysFromKeyMap() + "] To Context : " + this.MappingContext.GetName());
        this.MappingContext.Mappings = oldmapps;


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

    }

}
class MappingData_COMPILED extends MappingData { };

