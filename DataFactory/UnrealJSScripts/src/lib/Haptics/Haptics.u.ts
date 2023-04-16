import { CreateNewObject, CreateObject, bool } from '../Generics';
import { DFLOG_ToConsole, DFLOG_ToScreen } from "../Log";

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
 *
 * Using generics because the members of this class objects are being reset to undefined, probably some issue with GC,
 * bLoop = true might not work properly.
//  */

/**
 *
 * Since there were issues with binding memebers of this class
 *
 * moved this class to C++ as AHapticsBindingActor
 *
 */

// @UCLASS()
// class UJS_HapticsObject extends Actor
// {
//     /** */
//     @UPROPERTY(EditAnywhere, VisibleAnywhere)
//     public HapticEffect: HapticFeedbackEffect_Base;

//     /** */
//     @UPROPERTY(EditAnywhere, VisibleAnywhere)
//     public hand: EControllerHand = EControllerHand.AnyHand;

//     /** */
//     @UPROPERTY(EditAnywhere, VisibleAnywhere)
//     public Scale: number = 1;

//     /** */
//     @UPROPERTY(EditAnywhere, VisibleAnywhere)
//     public bLoop: bool = false;


//     @UFUNCTION()
//     PlayHaptics(Axis: number): void
//     {

//         DFLOG_ToConsole("Playing JS Haptics on : " + this.hand);
//         GWorld.GetPlayerController(0).PlayHapticEffect(this.HapticEffect, this.hand, this.Scale, this.bLoop);

//     }



// }

// export default UJS_HapticsObject;

