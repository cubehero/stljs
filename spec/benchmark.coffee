fs = require("fs")
parser = require("../lib/parser/jison_stl_parser").parser
parser2 = require("../lib/parser/stl_parser")
fileName = "./spec/fixtures/coffeecup/current.stl"
console.log "Comparing the two parsers..."
fs.readFile fileName, (err, data) ->
  console.time "jison parser"
  csgObj = parser.parse(data.toString())
  console.timeEnd "jison parser"
  solid1 = csgObj[0].polygons
  console.time "handcoded parser"
  parser2.parse fs.createReadStream(fileName), (err, solid2) ->
    console.timeEnd "handcoded parser"
    console.log "done!"


