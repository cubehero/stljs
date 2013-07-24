stl = require("../lib/stljs")
child_proc = require("child_process")

test_convert_stl = ->
  fileName = __dirname + "/fixtures/coffeecup/current.stl"
  stl.imageify fileName, { dst: 'ascii.png' }, (err) ->
    throw err if err?
    console.log "done rendering stl to png"

  fileName = __dirname + "/fixtures/binary.stl"
  stl.imageify fileName, { dst: 'binary.png' }, (err) ->
    throw err if err?
    console.log "done rendering binary"

  fileName = __dirname + "/fixtures/flat.stl"
  stl.imageify fileName, { dst: 'flat.png' }, (err) ->
    throw err if err?
    console.log "done rendering flat"

  fileName = __dirname + "/fixtures/tall.stl"
  stl.imageify fileName, { dst: 'tall.png' }, (err) ->
    throw err if err?
    console.log "done rendering tall"

  fileName = __dirname + "/fixtures/offset.stl"
  stl.imageify fileName, { dst: 'offset.png' }, (err) ->
    throw err if err?
    console.log "done rendering offset"

  fileName = __dirname + "/fixtures/povray_cmd.stl"
  stl.imageify fileName, { dst: 'povray_cmd.png' }, (err) ->
    throw err if err?
    console.log "done rendering povray_cmd"

test_convert_stl()
