/// <reference path="../../typings/_part_3_ue.d.ts">/>

@UCLASS()
class JS_FreeChar_Base extends Character
{
    CameraBoom: SpringArmComponent;

    FollowCamera;


    constructor(GWorld: World, Location: Vector)
    {
        super(GWorld, Location);

        this.bReplicates = true;
        this.bReplicateMovement = true;


        this.CapsuleComponent.SetCapsuleSize(42, 96, false);
        this.CapsuleComponent.SetHiddenInGame(true, false);
        this.bUseControllerRotationPitch = false;
        this.bUseControllerRotationRoll = false;
        this.bUseControllerRotationYaw = false;

        this.CameraBoom = new SpringArmComponent();
        this.CameraBoom.AttachParent = this.CapsuleComponent;
        this.CameraBoom.TargetArmLength = 300;
        this.CameraBoom.bUsePawnControlRotation = false;

        this.FollowCamera = new CameraComponent();
        this.FollowCamera.bUsePawnControlRotation = false;
        this.FollowCamera.AttachParent = this.CameraBoom;

        this.Mesh.K2_SetWorldLocation(Vector.MakeVector(0, 0, -97), false);
        this.Mesh.K2_SetWorldRotation(Rotator.MakeRotator(0, 0, 270), false);

        this.bindInputs();
    }


    public bindInputs(): void
    {

        // Axis name, Function name Bindings
        let AxisBindings = new Map<string, string>();

        AxisBindings.set("LookRight_InputAxis", "LookRight");
        AxisBindings.set("LookUp_InputAxis", "LookUp");
        AxisBindings.set("MoveForward_InputAxis", "MoveForward");
        AxisBindings.set("MoveRight_InputAxis", "MoveRight");
        AxisBindings.set("MoveUp_InputAxis", "MoveUp");

        /** add new bindings here
         * 
         * before for loop
         */





        for (const [key, value] of AxisBindings.entries())
        {
            this.SetAxisBinding(key, value);
        }

    }
    private SetAxisBinding(AxisName: string, FunctionName: string)
    {
        DataFactoryBPLibrary.AddInputBinding(this,
            AxisName, FunctionName,
            EInputBindingSupportedTypes.AxisBinding,
            EInputEvent.IE_Axis, null);

    }

    ReceiveBeginPlay()
    {
        super.ReceiveBeginPlay();
    }


    @UFUNCTION()
    LookRight(value: number)
    {

        if (value !== 0)
        {
            this.AddControllerYawInput(value);
        }
    }

    @UFUNCTION()
    LookUp(value: number)
    {

        if (value !== 0)
        {
            this.AddControllerPitchInput(value * -1);
        }
    }

    @UFUNCTION()
    MoveForward(value: number)
    {

        if (value !== 0)
        {
            const rotation = this.GetControlRotation();
            rotation.Pitch = 0;
            rotation.Roll = 0;

            const forwardVector = rotation.GetForwardVector();

            this.AddMovementInput(forwardVector, value, false);
        }
    }

    @UFUNCTION()
    MoveUp(value: number)
    {
        if (value !== 0)
        {
            const rotation = this.GetControlRotation();
            rotation.Pitch = 0;
            rotation.Roll = 0;

            const forwardVector = rotation.GetForwardVector();

            this.AddMovementInput(forwardVector, value, false);
        }
    }


    @UFUNCTION()
    MoveRight(value: number)
    {
        if (value !== 0)
        {
            const rotation = this.GetControlRotation();
            rotation.Pitch = 0;
            rotation.Roll = 0;

            const rightVector = rotation.GetRightVector();

            this.AddMovementInput(rightVector, value, false);
        }
    }

}

export default JS_FreeChar_Base;


