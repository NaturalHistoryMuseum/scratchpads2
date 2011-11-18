// Why doesn't Google already do this?
for(i in google.maps) {
  // Note, I'm assuming that functions starting with "_" shouldn't be messed
  // with.
  if(typeof (google.maps[i]) === 'function' && i[0] !== '_') {
    eval("google.maps." + i + ".prototype.getClass = function(){return \"" + i + "\"}");
  }
}