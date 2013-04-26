stl_pov = require('../lib/to/povray')
fs = require('fs')

#test_pov_convert = () ->
#  fileName = "spec/fixtures/coffeecup/current.stl"
#
#  console.time('stl2pov')
#  stl_pov.convert(fs.createReadStream(fileName),
#    (err, output) ->
#      console.timeEnd('stl2pov')
#      console.log output.length
#    , ((err, povPolygon) -> ))
#
#test_pov_convert()

stl_image = require('../lib/to/image')
fs = require('fs')

test_image_convert = () ->
  fileName = "spec/fixtures/coffeecup/current.stl"

  console.time('stl2img')
  stl_image.convert(fs.createReadStream(fileName), { height: 300, width: 400 },
    (err, output) ->
      console.timeEnd('stl2img')
      console.log output
  )

test_image_convert()
