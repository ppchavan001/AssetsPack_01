import { CreateObject } from "../Generics";
import { DFLOG_ToConsole, DFLOG_ToScreen } from "../Log";
import { HapticsOnMovementStarted } from "./Curves/HapticsOnMovementStarted";

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
class UJS_Haptics_COMPILED extends UJS_Haptics { }


/**
 * Dont need to spawn this actor unless trying to bind the UFUNCTION to InputAction,
 * call UJSHaptics.PlayHaptics directly if not binding UFUNCTION.
 */
@UCLASS()
class UJS_HapticsObject extends Actor
{
    private HapticEffect: HapticFeedbackEffect_Base;
    public hand: EControllerHand = EControllerHand.Right;
    public Scale: number = 1;
    public bLoop: boolean = false;

    private HapticEffectClass: typeof HapticFeedbackEffect_Base;


    /** Specify hand associated with this haptics object */
    constructor(GWorld: World, Location: Vector)
    {
        super(GWorld, Location);
        /** 
         * Do not build in ctor
         * 
         * class variable is not set here.
         */
        //this.RebuildHapticCurve();
    }

    // ReceiveBeginPlay()
    // {
    //     super.ReceiveBeginPlay();
    //     this.RebuildHapticCurve();

    // }

    ReceiveDestroyed(): void
    {
        DFLOG_ToConsole("JS Haptics binding actor destroyed for  : " + this.hand + " " + this.GetName());
        super.ReceiveDestroyed();
    }

    SetHapticEffectClass(HapticClass: typeof HapticFeedbackEffect_Base)
    {
        this.HapticEffectClass = HapticClass;
        this.RebuildHapticCurve();
    }

    RebuildHapticCurve(): void
    {
        let obj = CreateObject(this.HapticEffectClass);
        if (obj != null)
        {
            this.HapticEffect = obj;
        }
        else
        {
            /** TODO : figure out why this.HapticEffectClass == undefined after a while. and remove hardcoded class.*/
            this.HapticEffect = new HapticsOnMovementStarted();
        }
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

