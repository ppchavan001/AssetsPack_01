// @ts-nocheck

@UCLASS()
class JS_Pawn_Base extends Character
{
    @UPROPERTY(EditAnywhere)
    CameraBoom: SpringArmComponent;

    @UPROPERTY(EditAnywhere)
    FollowCamera;

    /**
     * The way to get started is to quit talking and begin doing.
     *
     * MyCharacter's Constructor
     */
    constructor(GWorld: World, Location: Vector)
    {
        super(GWorld, Location);

        this.bReplicates = true;
        this.bReplicateMovement = true;


        this.CapsuleComponent.SetCapsuleSize(42, 96, false);

        this.bUseControllerRotationPitch = false;
        this.bUseControllerRotationRoll = false;
        this.bUseControllerRotationYaw = false;

        this.CharacterMovement.bOrientRotationToMovement = true;
        this.CharacterMovement.RotationRate = Rotator.MakeRotator(0, 0, 540);
        this.CharacterMovement.JumpZVelocity = 600;
        this.CharacterMovement.AirControl = 0.2;
        this.CharacterMovement.MaxAcceleration = 2048;

        this.CameraBoom = new SpringArmComponent("CameraBoom");
        this.CameraBoom.AttachParent = this.CapsuleComponent;
        this.CameraBoom.TargetArmLength = 300;
        this.CameraBoom.bUsePawnControlRotation = true;

        this.FollowCamera = new CameraComponent("FollowCamera");
        this.FollowCamera.bUsePawnControlRotation = false;
        this.FollowCamera.AttachParent = this.CameraBoom;

        this.Mesh.K2_SetWorldLocation(Vector.MakeVector(0, 0, -97), false);
        this.Mesh.K2_SetWorldRotation(Rotator.MakeRotator(0, 0, 270), false);
    }


    ReceiveBeginPlay()
    {
        super.ReceiveBeginPlay();
    }

    @KEYBIND(BindAxis, "LookRight_InputAxis")
    Turn(value: float)
    {
        if (value !== 0)
        {
            this.AddControllerYawInput(value);
        }
    }

    @KEYBIND(BindAxis, "LookUp_InputAxis")
    LookUp(value: float)
    {

        console.log(value);
        if (value !== 0)
        {
            this.AddControllerPitchInput(value);
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

new JS_Pawn_Base(GWorld);

export default JS_Pawn_Base;
