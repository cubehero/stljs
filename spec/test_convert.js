stl = require('../lib/stl')
child_proc = require('child_process')

var test_convert_stl = function() {
  var fileName = "current.stl";

  child_proc.exec("cd spec/fixtures/coffeecup; ../../../lib/render_stl/render_stl.sh " + fileName + " /tmp",
      function(err, stdout, stderr) {
    console.log(err);
    console.log("---");
    console.log(stdout);
    console.log("---");
    console.log(stderr);
    console.log("done rendering stl to png");
  });
}

test_convert_stl();
