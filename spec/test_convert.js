stl = require('../lib/stl')
child_proc = require('child_process')

var test_convert_stl = function() {
  var fileName = "spec/fixtures/coffeecup/current.stl";

  stl.file2PNG(fileName, function(err) {
    if (err != null) throw err;
    console.log("done rendering stl to png");
  });
}

test_convert_stl();
