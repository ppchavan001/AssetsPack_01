/// <reference path="typings/ue.d.ts">/>
// ; typing info for auto-completion in Visual Studio Code

"use strict";

function main() {
  //Context.CreateInspector(9222)
  // create a new actor
  // ; new ActorClass(world{World}, location{Vector}, rotation{Rotator})
  let actor = new TextRenderActor(GWorld, { X: 100, Z: 100 }, { Yaw: 180 });

  // initialie its text render component
  actor.TextRender.SetHorizontalAlignment("EHTA_Center");
  actor.TextRender.SetText("Hello World");

  let actor2 = new TextRenderActor(GWorld, { X: 100, Z: 120 }, { Yaw: 180 });

  // mark dirty to update changes during debug
  actor2.TextRender.MarkRenderStateDirty();

  // clean up the mess
  return function () {
    actor2.DestroyActor();
    actor.DestroyActor();
  };
}

// bootstrap to initiate live-reloading dev env.
try {
  module.exports = () => {
    let cleanup = null;

    // wait for map to be loaded.
    process.nextTick(() => (cleanup = main()));

    // live-reloadable function should return its cleanup function
    return () => cleanup();
  };
} catch (e) {
  require("bootstrap")("helloWorld");
}
