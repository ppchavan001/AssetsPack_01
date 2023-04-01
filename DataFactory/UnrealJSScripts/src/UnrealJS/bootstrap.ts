import path from "path"

(function (global)
{
    "use strict"

    module.exports = function (filename: string)
    {
        //        Context.WriteDTS(Context.Paths[0] + 'typings/ue.d.ts')
        //        Context.WriteAliases(Context.Paths[0] + 'aliases.js')



        // Generate typings

        // in dist dir
        // Context.WriteDTS(path.join(__dirname, './typings/ue.d.ts'), true);
        // Context.WriteAliases(path.join(__dirname, '/aliases.js'));

        // in src
        // Context.WriteDTS(path.join(__dirname, '../../src/UnrealJS/typings/ue.d.ts'), true);
        // Context.WriteAliases(path.join(__dirname, '../../src/UnrealJS/aliases.js'));

        Context.RunFile('aliases.js')
        Context.RunFile('polyfill/unrealengine.js')
        Context.RunFile('polyfill/timers.js')

        require('devrequire')(filename)
    }
})(this)
