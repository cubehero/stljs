stl_parser = require('../parser/stl_parser')

class Stl

class Stl.PovRay

  constructor: () ->

  _povHeaders: (name) ->
    "#declare #{name} = mesh {\n"

  _povFooters: () -> "}"

  convertFile: (filePath, callback, progressCb) ->
    output = ""

    stl_parser.parseFile filePath,
      (err, polygons, name) =>
        (callback(err); return) if err?

        unique_name = '__' + name + '__'
        output += @._povFooters()
        callback(null, output, unique_name)
      , (err, polygon, name) =>

        unique_name = '__' + name + '__'
        if output.length is 0
          output += @._povHeaders(unique_name)

        povPolygon = @.convertPolygon(polygon)
        output += povPolygon
        progressCb(err, povPolygon, unique_name) if progressCb?

  convertPolygon: (polygon) ->
    output = ""

    output += "  triangle {\n"

    for vertex, idx in polygon.verticies
      output += "    <#{vertex[0]}, #{-vertex[1]}, #{vertex[2]}>"
      output += ",\n" unless idx is (polygon.verticies.length - 1)

    output += "  }\n"
    return output

module.exports = new Stl.PovRay()
