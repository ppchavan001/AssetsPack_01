/// <reference path="../../typings/_part_3_ue.d.ts">/>

@UCLASS()
class JS_FreeChar_Base extends Character
{
    @UPROPERTY(EditAnywhere)
    CameraBoom: SpringArmComponent;

    @UPROPERTY(EditAnywhere)
    FollowCamera;


    constructor(GWorld: World, Location: Vector)
    {
        super(GWorld, Location);

        this.bReplicates = true;
        this.bReplicateMovement = true;


        this.CapsuleComponent.SetCapsuleSize(42, 96, false);
        this.CapsuleComponent.SetHiddenInGame(true);
        this.bUseControllerRotationPitch = false;
        this.bUseControllerRotationRoll = false;
        this.bUseControllerRotationYaw = false;

        this.CameraBoom = new SpringArmComponent("CameraBoom");
        this.CameraBoom.AttachParent = this.CapsuleComponent;
        this.CameraBoom.TargetArmLength = 300;
        this.CameraBoom.bUsePawnControlRotation = false;

        this.FollowCamera = new CameraComponent("FollowCamera");
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
            EInputEvent.IE_Axis);

    }

    ReceiveBeginPlay()
    {
        super.ReceiveBeginPlay();
    }


    @KEYBIND(BindAxis, "LookRight_InputAxis")
    LookRight(value: float)
    {

        if (value !== 0)
        {
            this.AddControllerYawInput(value);
        }
    }

    @KEYBIND(BindAxis, "LookUp_InputAxis")
    LookUp(value: float)
    {

        if (value !== 0)
        {
            this.AddControllerPitchInput(value * -1);
        }
    }

    @KEYBIND(BindAxis, "MoveForward_InputAxis")
    MoveForward(value: float)
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

    @KEYBIND(BindAxis, "MoveUp_InputAxis")
    MoveUp(value: float)
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


    @KEYBIND(BindAxis, "MoveRight_InputAxis")
    MoveRight(value: float)
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

    @UFUNCTION(Server, Unreliable)
    Server_Attack()
    {
        this.Attack();
    }

    @KEYBIND(BindAction, "Attack", IE_RELEASED)
    Attack()
    {
        if (!this.HasAuthority())
        {
            this.Server_Attack();
            return;
        }
    }
}

export default JS_FreeChar_Base;


