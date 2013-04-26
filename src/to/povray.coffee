stl_parser = require('../parser/stl_parser')

class Stl

class Stl.PovRay

  constructor: () ->

  _povHeaders: (name) ->
    "#declare #{name} = mesh {\n"

  _povFooters: () -> "}"

  convert: (input, callback, progressCb) ->
    output = ""

    stl_parser.parse input,
      (err, polygons, name) =>
        (callback(err); return) if err?
        output += @._povFooters()
        callback(null, output, name)
      , (err, polygon, name) =>
        if output.length is 0
          output += @._povHeaders(name)
        povPolygon = @.convertPolygon(polygon)
        output += povPolygon
        progressCb(err, povPolygon, name) if progressCb?

  convertPolygon: (polygon) ->
    output = ""

    output += "  triangle {\n"

    for vertex, idx in polygon
      output += "    <#{vertex.points[0]}, #{vertex.points[1]}, #{vertex.points[2]}>"
      output += ",\n" unless idx is (polygon.length - 1)

    output += "  }\n"
    return output

module.exports = new Stl.PovRay()
