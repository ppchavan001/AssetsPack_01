/// <reference path="../typings/ue.d.ts">/>

@UCLASS()
class TestClass extends Actor
{
	/**
	 * What do you get when you cross a snowman with a vampire? Frostbite.
	 *
	 * TestClass's Constructor
	 */
	constructor(GWorld: World)
	{
		super(GWorld);
	}
}

//export default TestClass;

function main()
{
	//Context.CreateInspector(9222)
	// create a new actor
	// ; new ActorClass(world{World}, location{Vector}, rotation{Rotator})
	let actor = new TextRenderActor(
		GWorld,
		Vector.MakeVector(100, 0, 100),
		Rotator.MakeRotator(0, 0, 180)
	);

	// initialie its text render component
	actor.TextRender.SetHorizontalAlignment("EHTA_Center");
	actor.TextRender.SetText("Hello World : " + (__filename));

	let actor2 = new TextRenderActor(
		GWorld,
		Vector.MakeVector(100, 0, 120),
		Rotator.MakeRotator(0, 0, 180)
	);

	actor2.TextRender.SetText("@ppchavan001");

	// mark dirty to update changes during debug
	actor2.TextRender.MarkRenderStateDirty();

	// clean up the mess
	return function ()
	{
		actor2.DestroyActor();
		actor.DestroyActor();
	};
}


import path from "path";
// bootstrap to initiate live-reloading dev env.
let filename = path.basename(__filename);
filename = filename.replace(path.extname(filename), "");
// try
// {
// 	module.exports = () =>
// 	{
// 		let cleanup: Function | null = null;

// 		// wait for map to be loaded.
// 		process.nextTick(() => (cleanup = main()));

// 		// live-reloadable function should return its cleanup function
// 		return () => (<Function>cleanup)();
// 	};

// } catch (e)
// {
// 	require("../bootstrap")(filename);
// }

try
{
	module.exports = () =>
	{
		let cleanup: Function;

		// wait for map to be loaded.
		process.nextTick(() => (cleanup = main()));

		// live-reloadable function should return its cleanup function
		return () => cleanup();
	};
} catch (e)
{
	filename = path.basename(__filename);
	filename = filename.replace(path.extname(filename), "");
	require("../bootstrap")('../out/TestClass.u');
}
