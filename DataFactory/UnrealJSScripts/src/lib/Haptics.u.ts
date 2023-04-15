import { DFLOG_ToConsole, DFLOG_ToScreen } from "./Log";


class UJS_Haptics extends UObject
{

    public static PlayHapticFeedback(
        HapticEffect: HapticFeedbackEffect_Base,
        hand: EControllerHand = EControllerHand.Right,
        Scale: number = 1.0,
        bLoop: boolean = false
    ): void
    {
        DFLOG_ToConsole("Playing JS Haptics on : " + hand);
        GWorld.GetPlayerController(0).PlayHapticEffect(HapticEffect, hand, Scale, bLoop);
    }

}
/**
 * Dont need to spawn this actor unless trying to bind the UFUNCTION to InputAction,
 * call UJSHaptics.PlayHaptics directly if not binding UFUNCTION.
 */
@UCLASS()
class UJS_HapticsObject extends Actor
{
    public HapticEffect: HapticFeedbackEffect_Curve;
    public hand: EControllerHand = EControllerHand.Right;
    public Scale: number = 1;
    public bLoop: boolean = false;




    /** Specify hand associated with this haptics object */
    constructor(GWorld: World, Location: Vector)
    {
        super(GWorld, Location);
        this.RebuildHapticCurve();
    }

    ReceiveBeginPlay()
    {
        super.ReceiveBeginPlay();
        this.RebuildHapticCurve();

    }

    ReceiveDestroyed(): void
    {
        DFLOG_ToConsole("JS Haptics binding actor destroyed for  : " + this.hand);
        super.ReceiveDestroyed();
    }

    RebuildHapticCurve(): void
    {

        this.HapticEffect = new HapticFeedbackEffect_Curve();

        const richCurve = new RichCurve();

        let k1 = new RichCurveKey();
        k1.Time = 0;
        k1.Value = 0.1;

        let k2 = new RichCurveKey();
        k2.Time = 0.3;
        k2.Value = 0;

        // Add some keyframes to the RichCurve
        richCurve.Keys = [k1, k2];

        // Create a new RuntimeFloatCurve using the RichCurve
        const runtimeFloatCurve = new RuntimeFloatCurve();
        runtimeFloatCurve.EditorCurveData = richCurve;

        this.HapticEffect.HapticDetails.Frequency = runtimeFloatCurve;
        this.HapticEffect.HapticDetails.Amplitude = runtimeFloatCurve;
    }

    @UFUNCTION()
    PlayHaptics(Axis: number): void
    {
        if (this.HapticEffect == undefined)
        {
            this.RebuildHapticCurve();
        }
        UJS_Haptics.PlayHapticFeedback(this.HapticEffect, this.hand, this.Scale, this.bLoop);
    }



}

export default UJS_HapticsObject;

