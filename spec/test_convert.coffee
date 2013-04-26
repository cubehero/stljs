stl = require("../lib/stljs")
child_proc = require("child_process")

test_convert_stl = ->
  fileName = __dirname + "/fixtures/coffeecup/current.stl"
  stl.imageify fileName, { dst: 'ascii.png' }, (err) ->
    throw err  if err?
    console.log "done rendering stl to png"

  fileName = __dirname + "/fixtures/binary.stl"
  stl.imageify fileName, { dst: 'binary.png' }, (err) ->
    throw err if err?
    console.log "done rendering binary"

test_convert_stl()
