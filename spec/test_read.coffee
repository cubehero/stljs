fs = require('fs')
stl = require("../lib/stljs")

#test_ascii_read_stl = () ->
#  fileName = "spec/fixtures/coffeecup/current.stl"
#  console.time "parsing"
#  stl.readFile fileName, (err, solid, name) ->
#    (console.error(err); return) if err?
#
#    console.log name
#    console.log solid.length
#    console.timeEnd "parsing"
#    console.log "done reading polygons\n"
#  #, (err, polygon) ->
#  #  console.log polygon
#
#test_ascii_read_stl()

test_binary_read_stl = () ->
  fileName = "spec/fixtures/binary.stl"
  console.time "parsing"
  stl.readFile fileName, (err, solid, name) ->
    (console.error(err); return) if err?

    console.log name
    console.log solid.length
    console.timeEnd "parsing"
    console.log "done reading polygons"
  #, (err, polygon) ->
  #  console.log polygon

test_binary_read_stl()

