
@UCLASS(BlueprintComponent)
class MyCharacter extends Character
{
    @UPROPERTY(EditAnywhere)
    CameraBoom: SpringArmComponent;

    @UPROPERTY(EditAnywhere)
    FollowCamera: CameraComponent;

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

        this.CameraBoom = SpringArmComponent.CreateDefaultSubobject("CameraBoom");
        this.CameraBoom.AttachParent = this.CapsuleComponent;
        this.CameraBoom.TargetArmLength = 300;
        this.CameraBoom.bUsePawnControlRotation = true;

        this.FollowCamera = CameraComponent.CreateDefaultSubobject("FollowCamera");
        this.FollowCamera.bUsePawnControlRotation = false;
        this.FollowCamera.AttachParent = this.CameraBoom;

        this.Mesh.SetSkeletalMesh(SkeletalMesh.Load('/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'), true);
        this.Mesh.SetAnimClass(AnimBlueprint.Load('/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP').GeneratedClass);

        this.Mesh.K2_SetWorldLocation(Vector.MakeVector(0, 0, -97), false);
        this.Mesh.K2_SetWorldRotation(Rotator.MakeRotator(0, 0, 270), false);
    }

    ReceiveBeginPlay()
    {
        super.ReceiveBeginPlay();

        // Posses on spawn
        GWorld.GetPlayerController(0).Possess(this);
    }

    @KEYBIND(BindAxis, "Turn")
    Turn(value: float)
    {
        if (value !== 0)
        {
            this.AddControllerYawInput(value);
        }
    }

    @KEYBIND(BindAxis, "LookUp")
    LookUp(value: float)
    {
        if (value !== 0)
        {
            this.AddControllerPitchInput(value);
        }
    }

    @KEYBIND(BindAxis, "MoveForward")
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

    @KEYBIND(BindAxis, "MoveRight")
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

new MyCharacter(GWorld, Vector.MakeVector(0, 0, 500);

export default MyCharacter;
