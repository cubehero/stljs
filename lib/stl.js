var fs = require('fs');
var path = require('path');
var child_proc = require('child_process');
var parser = require('./stl_parser');

var STL = function() {};

/*
 * Asynchronously read an STL string, and converts to polygon list.
 * Don't use this for large files. Use readFile instead, which takes
 * advantage of node streaming libraries, and also lessens the memory footprint
 *
 * TODO probably deprecate this or change to read buffers instead
 * or have a special readString that calls read
 *
 * Options:
 *   type - 'asc' or 'bin'. defaults to ascii
 */
STL.readString = function(stlString, options, progressCallback, doneCallback) {
  if (!(typeof options !== "undefined" && options !== null))
    options = { type: 'asc' };

  // currently only reads ascii.
  // TODO read binary STL files
  parser.parse(stlString, progressCallback, doneCallback);
}

/*
 * Asynchronously read an STL file
 *
 * Options:
 *   type - 'asc' or 'bin'. defaults to ascii
 */
STL.readFile = function(fileName, options, progressCallback, doneCallback) {
  if (!(typeof options !== "undefined" && options !== null))
    options = { type: 'asc' };

  STL.readStream(fs.createReadStream(fileName), options,
      progressCallback, doneCallback);
}

/*
 * Asynchronously read a stream of STL file
 * TODO maybe combine readString() and readStream()
 *
 * Options:
 *   type - 'asc' or 'bin', defaults to ascii
 */
STL.readStream = function(stream, options, progressCallback, doneCallback) {
  if (!(typeof options !== "undefined" && options !== null))
    options = { type: 'asc' };

  parser.parse(stream, progressCallback, doneCallback);
}

/* Converts an STL file to PNG
 *
 * Do not use. Currently a hack
 */
STL.file2PNG = function(filePath, callback) {
  var renderer = path.resolve("lib/render_stl/render_stl.sh")
  var pathname = path.dirname(filePath);
  var basename = path.basename(filePath);

  child_proc.exec("cd " + pathname + "; " + renderer + " " + basename + " /tmp",
    function(err, stdout, stderr) {
      callback(err);
  });
}

/*
 * Asynchronously writes an STL file
 *
 * It runs through the list of polygons and writes out to file
 */
STL.writeFile = function(fileName, solidName, polygons, callback) {
  var stream = fs.createWriteStream(fileName);

  var writePolygon = function(vertices, normal) {
    stream.write("  facet normal ".concat(normal.x, " ", normal.y, " ", normal.z, "\n"));
    stream.write("    outer loop\n");

    // console.log("normal: " + [normal.x, normal.y, normal.z].join(", "));
    // console.log("vertices: " + vertices.length);

    for (var j = 0, jlen = vertices.length; j < jlen; j++) {
      var vertex = vertices[j];
      stream.write("      vertex ".concat(vertex.pos.x, " ",
                                          vertex.pos.y, " ",
                                          vertex.pos.z, "\n"));
      // console.log(" " + [vertex.pos.x, vertex.pos.y, vertex.pos.z].join(", "));
    }

    stream.write("    endloop\n");
    stream.write("  endfacet\n");

    // console.log("\n");
  }

  stream.once('open', function(fd) {
    stream.write("solid " + solidName + "\n");
    for (var i = 0, ilen = polygons.length; i < ilen; i++) {
      var polygon = polygons[i];
      var normal = polygon.vertices[0].normal;
      // TODO try making normal vector a unit vector

      // splits up polygons that have more than 3 vertices
      // It takes one vertex as the common point for all triangles, and makes triangles
      // from the polygon
      for (var v = 1, vlen = polygon.vertices.length; v < vlen - 1; v++) {
        writePolygon([polygon.vertices[0],
                      polygon.vertices[v],
                      polygon.vertices[v + 1]], normal);
      }
    }
    stream.write("endsolid " + solidName + "\n");
    if (callback) callback(null);
  });
}

module.exports = STL;
