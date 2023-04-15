

export class HapticsOnMovementStarted extends HapticFeedbackEffect_Curve
{
    constructor()
    {
        super();


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

        this.HapticDetails.Frequency = runtimeFloatCurve;
        this.HapticDetails.Amplitude = runtimeFloatCurve;
    }


}

class HapticsOnMovementStarted_COMPILED extends HapticsOnMovementStarted { }
