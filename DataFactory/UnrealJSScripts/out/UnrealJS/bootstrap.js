"use strict";

(function (global)
{
  "use strict";

  module.exports = function (filename)
  {
    //        Context.WriteDTS(Context.Paths[0] + 'typings/ue.d.ts')
    //        Context.WriteAliases(Context.Paths[0] + 'aliases.js')



    // Generate typings
    //Context.WriteDTS(path.join(__dirname, './typings/ue.d.ts'), true);
    //Context.WriteAliases(path.join(__dirname, '/aliases.js'));

    Context.RunFile('aliases.js');
    Context.RunFile('polyfill/unrealengine.js');
    Context.RunFile('polyfill/timers.js');

    require('devrequire')(filename);
  };
})(void 0);
//# sourceMappingURL=bootstrap.js.map