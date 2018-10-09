var modules = {};

window.define = function(name, deps, cb) {
  modules[name] = cb();
};
window.define.amd = true;

window.require = function(deps, module) {
  module.apply(null, getModules(deps));

  function getModules(names) {
    return names.map(function(name) {
      return modules[name];
    });
  }
};
