var fs = require('fs');

var parser = require('../lib/jison_stl_parser').parser;
var parser2 = require('../lib/stl_parser');

fileName = './spec/fixtures/coffeecup/current.stl';

console.log("Comparing the two parsers...");

fs.readFile(fileName, function(err, data) {
  console.time('jison parser');
  var csgObj = parser.parse(data.toString());
  console.timeEnd('jison parser');
  var solid1 = csgObj[0].polygons;

  var solid2 = [];
  console.time("handcoded parser");
  parser2.parse(fs.createReadStream(fileName), function(err, polygon) {
    solid2.push(polygon);
  }, function(err) {
    console.timeEnd("handcoded parser");
    console.log('done!');
  });

});



