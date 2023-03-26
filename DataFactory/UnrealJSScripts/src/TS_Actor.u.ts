/// <reference path="../typings/ue.d.ts">/>

@UCLASS()
class TS_Actor extends Actor {
  /**
   * Did you hear about the restaurant on the moon? Great food, no atmosphere.
   *
   * TS_Actor's Constructor
   */
  constructor(GWorld: World) {
    super(GWorld);
  }

  ReceiveBeginPlay(): void {
    super.ReceiveBeginPlay();
    console.log("TS_Actor::ReceiveBeginPlay");
  }
}

export default TS_Actor;
