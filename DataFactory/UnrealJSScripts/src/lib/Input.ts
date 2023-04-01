

export function SetAxisBinding(obj: UObject, AxisName: string, FunctionName: string)
{
    DataFactoryBPLibrary.AddInputBinding(obj,
        AxisName, FunctionName,
        EInputBindingSupportedTypes.AxisBinding,
        EInputEvent.IE_Axis, null);

}
