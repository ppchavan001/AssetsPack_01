/// <reference path="../../../Content/Scripts/typings/ue.d.ts">/>
import path from "path";

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
	actor.TextRender.SetText("" + (__filename));

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


/*
 * Get directory
	
*/


const __scriptPathRelative = Context.GetDir('GameContent') + 'Scripts';
const __ScriptPathAbs = path.resolve(__scriptPathRelative);
const CurrentFile = path.resolve(__filename);
const CurrentDir = path.resolve(__dirname);
let relativePath1 = path.relative(__ScriptPathAbs, CurrentFile);
DataFactoryBPLibrary.DF_PrintString(undefined, "Out dir : " + CurrentDir, EDataFactoryLogVerbosity.Warning, true, true, 5.0, 3);


// bootstrap to initiate live-reloading dev env.

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

	//Context.Paths.push("A:\Project\AssetsPack_01\DataFactory\UnrealJSScripts\out");
	//require("../../../Content/Scripts/bootstrap")('../../DataFactory/UnrealJSScripts/out/TestClass.u.js');
	require("bootstrap")('../../DataFactory/UnrealJSScripts/out/TestClass.u');
	//require("../bootstrap.js")(relativePath1);
}
