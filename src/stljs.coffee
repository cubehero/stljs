fs = require('fs')
path = require('path')
child_proc = require('child_process')

stl_parser = require('./parser/stl_parser')
to_image = require('./to/image')

class Stl

# Asynchronously read an STL string, and converts to polygon list.
# 
# @params - input - a string or a buffer
# @params = next (function)
# @params - progressCb (function)
#
# Stl.read(string_or_buffer,
#   , (err, polygons, modelName) ->
#     # do stuff
#   , (err, polygon, modelName) ->
#     # do stuff on every iteration
# )
#
#Stl.read = (input, callback, progressCb) ->
#  stl_parser.parse(input, callback, progressCb)

#
# Asynchronously read an STL file
#
Stl.readFile = (filePath, callback, progressCb) ->
  stl_parser.parseFile(filePath, callback, progressCb)

# Converts an STL file to PNG
# 
# file2PNG(filePath, options, callback, [progressCb])
# 
# options parameter is required
# 
# options:
#   height:
#   width:
#   dst: desination. default "./result.png
# 
Stl.file2PNG = (filePath, options, callback, progressCb) ->
  if options == null then options = {}
  if (options.width == null || typeof options.width == 'undefined')
    options.width = 800
  if (options.height == null || typeof options.height == 'undefined')
    options.height = 600

  to_image.convertFile(filePath, options, callback, progressCb)
  

# 
# Asynchronously writes an Stl file
# 
# It runs through the list of polygons and writes out to file
# 
Stl.writeFile = (fileName, solidName, polygons, callback) ->
  stream = fs.createWriteStream(fileName)

  writePolygon = (vertices, normal) ->
    stream.write("  facet normal #{normal.x} #{normal.y} #{normal.z}\n")
    stream.write("    outer loop\n")

    # console.log("normal: " + [normal.x, normal.y, normal.z].join(", "));
    # console.log("vertices: " + vertices.length);

    for vertex in verticies
      stream.write("      vertex #{vertex.pos.x} #{vertex.pos.y} #{vertex.pos.z} \n")
      # console.log(" " + [vertex.pos.x, vertex.pos.y, vertex.pos.z].join(", "));

    stream.write("    endloop\n")
    stream.write("  endfacet\n")

    # console.log("\n");

  stream.once('open', (fd) ->
    stream.write("solid #{solidName}\n")
    for polygon in polygons
      normal = polygon.vertices[0].normal
      # TODO try making normal vector a unit vector

      # splits up polygons that have more than 3 vertices
      # It takes one vertex as the common point for all triangles, and makes triangles
      # from the polygon
      for v in [1...polygon.verticies.length - 1]
        writePolygon([polygon.vertices[0],
                      polygon.vertices[v],
                      polygon.vertices[v + 1]], normal)

    stream.write("endsolid " + solidName + "\n")
    if (callback) then callback(null)
  )

module.exports = Stl
