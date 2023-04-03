

export function SetAxisBinding(obj: UObject, AxisName: string, FunctionName: string)
{
    DataFactoryBPLibrary.AddInputBinding(obj,
        AxisName, FunctionName,
        EInputBindingSupportedTypes.AxisBinding,
        EInputEvent.IE_Axis, null);

}

export function SetKeyBinding(obj: UObject, KeyName: string, FunctionName: string, InputEvent: EInputEvent = EInputEvent.IE_Released)
{
    DataFactoryBPLibrary.AddInputBinding(obj,
        KeyName, FunctionName,
        EInputBindingSupportedTypes.KeyBinding,
        InputEvent, null);

}


//export function SetActionBinding(obj: UObject, ActionName: string, FunctionName: string, InputEvent: EInputEvent = EInputEvent.IE_Released)
export function SetActionBinding(obj: UObject, ActionName: string, FunctionName: string, InputEvent: EInputEvent = EInputEvent.IE_Released)
{
    DataFactoryBPLibrary.AddInputBinding(obj,
        ActionName, FunctionName,
        EInputBindingSupportedTypes.ActionBinding,
        InputEvent, null);

}

export function SetAxisBindings(obj: UObject, AxisBindings: Map<string, string>)
{

    for (const [key, value] of AxisBindings.entries())
    {
        SetAxisBinding(obj, key, value);
    }

}
