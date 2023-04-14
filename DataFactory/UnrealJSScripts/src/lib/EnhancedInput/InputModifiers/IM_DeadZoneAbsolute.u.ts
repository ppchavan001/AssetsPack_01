
@UCLASS()
class IM_DeadZoneAbsolute extends InputModifierDeadZone
{

    /** 
     * Ignores all input values 
     * that are 
     * 1. lesser than UpperThreshold
     * 2. and greater than LowerThreshold
     *       
     * check ModifyRaw for exact implementation.
     * 
     */
    constructor(outer: UObject)
    {
        super(outer);
    }
    ModifyRaw(PlayerInput: EnhancedPlayerInput, CurrentValue: InputActionValue, DeltaTime: number): InputActionValue
    {
        if (
            CurrentValue.Conv_InputActionValueToAxis1D() > this.UpperThreshold ||
            CurrentValue.Conv_InputActionValueToAxis1D() < this.LowerThreshold
        )
        {
            return CurrentValue;
        }

        return new InputActionValue();

    }


}

export default IM_DeadZoneAbsolute;

