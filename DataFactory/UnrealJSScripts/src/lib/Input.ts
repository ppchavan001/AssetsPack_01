

export function SetAxisBinding(obj: UObject, AxisName: string, FunctionName: string)
{
    DataFactoryBPLibrary.AddInputBinding(obj,
        AxisName, FunctionName,
        EInputBindingSupportedTypes.AxisBinding,
        EInputEvent.IE_Axis, null);

}

export function SetAxisBindings(obj: UObject, AxisBindings: Map<string, string>)
{

    for (const [key, value] of AxisBindings.entries())
    {
        SetAxisBinding(obj, key, value);
    }

}
