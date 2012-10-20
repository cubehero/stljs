stl = require('../lib/stl')

var test_read_stl = function() {
  var fileName = "spec/fixtures/coffeecup/current.stl";
  console.time('parsing');

  stl.readFile(fileName, {}, function(err, polygon) {
    // console.log(polygon);
  }, function(err, solid) {
    console.log(solid.length);
    console.timeEnd('parsing');
    console.log("done reading polygons");
  });
}

test_read_stl();
