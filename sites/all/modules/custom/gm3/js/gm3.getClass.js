// Why doesn't Google already do this?
setTimeout(function(){
  console.log(google.maps);
  for(i in google.maps) {
    // Note, I'm assuming that functions starting with "_" shouldn't be messed
    // with.
    if(typeof (google.maps[i]) === 'function' && i[0] !== '_') {
      console.log('Adding get class function to ' + i);
      eval("google.maps." + i + ".prototype.getClass = function(){return \"" + i + "\"}");
    }
  }
}, 200);