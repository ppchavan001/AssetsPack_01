

/**
 *  returns a new object of parameter type 
 * 
 * ex.  HapticEffect = CreateObject(HapticEffectClass);
 *  HapticEffect = new object of type HapticEffectClass
*/

import { DFLOG, DFLOG_Error, DFLOG_ToConsole } from "./Log";

export function CreateObject<T>(clazz: new () => T): T | null
{

    if (clazz != undefined)
    {
        return new clazz();
    }

    DFLOG_Error("JS : Tried to create object of undefined type! " + clazz);

    return null;
}
