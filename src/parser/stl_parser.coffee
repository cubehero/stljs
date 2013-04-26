fs = require('fs')

class Stl

# Handcoded parser for STL files. This is the interface file for the actual classes
#
# -- NOTES: --
#
# Originally created a parser from jison, but it was running too slow.
#
# Will not use promises here, because we want it as fast as possible, and the format
# is simple enough that we can manage.
class Stl.Parser
  
  constructor: () ->
    @_reset()

  _reset: () ->
    @.normal = []
    @.verticies = []

  # synchronous method that tells us whether an input is a binary or ascii STL file
  #
  # ASCII STLs usually have 'solid' near the beginning of the file, but according to
  # thingiverse, there's often more than one binary file that breaks this.
  #
  # So we try to read off the binary format's prediction of the file length. If it 
  # correctly predicts the file length, it's probably a binary file
  isBinaryFile: (filePath) ->
    countBuf = new Buffer(4)

    fd = fs.openSync(filePath, 'r')
    bytesRead = fs.readSync(fd, countBuf, 0, 4, 80)
    fs.closeSync(fd)

    # 80 byte header
    # UInt32 count
    # 50 bytes per triangle
    predictedSize = 80 + 4 + 50 * countBuf.readUInt32LE(0)
    fileStat = fs.statSync(filePath)

    return predictedSize is fileStat.size

  # unused, since we don't know how to reset a stream. Once isBinary reads the stream, we can't
  # unwind it to read the file. 
  isBinary: (input, callback) ->
    totalLen = 0
    isFirstData = true
    predictedSize = null

    input.resume()
    input.on('data', (data) ->
      totalLen += data.length
      if isFirstData && totalLen > 84
        predictedSize = 80 + 4 + 50 * data.readUInt32LE(80)
        isFirstData = false
    )

    input.on('end', () ->
      if totalLen < 80
        callback(new Error("File is too short to be an STL file"))
      else
        callback(null, predictedSize is totalLen)
    )

    input.on('error', (err) ->
      callback(err)
    )

  # parses an STL file
  # 
  # @param - Stream / String - input
  # @param - function - callback
  # 
  # callback
  # @param - error - error Object
  # @param - Array - a list of polygons
  # @param - modelName - the name of model
  # 
  # progressCb
  # @param - error - error object
  # @param - polygon - array of vertices. 
  # 
  parseFile: (filePath, callback, progressCb) ->
    readStream = fs.createReadStream(filePath)
    readStream.pause()
    if @.isBinaryFile(filePath)
      parser = new Stl.BinaryParser()
    else
      parser = new Stl.AsciiParser()
    parser.parse(readStream, callback, progressCb)

  # imagine we need this, so we don't have to write to file in order to read an STL, and be able to read it
  # directly from a stream or buffer
  parse: (input, callback, progressCb) ->
    console.log "not yet implemented naked parser"


class Stl.AsciiParser extends Stl.Parser
  _detectName: (line) ->
    md = line.match(/^solid/)
    if md?
      md = line.match(/solid\s+(\w+)/)
      if md?
        return md[1]
      else
        return "_default_name_for_model"
    else
      return null

  # parses an STL file
  # 
  # @param - Stream / String / Buffer - input
  # @param - function - callback
  # 
  # callback
  # @param - error - error Object
  # @param - Array - a list of polygons
  # @param - modelName - the name of model
  # 
  # progressCb
  # @param - error - error object
  # @param - polygon - array of vertices. 
  # 
  parse: (input, callback, progressCb) ->
    solidModel = []
    modelName = null

    @.eachLine input,
      (line) =>
        name = @._detectName(line)
        modelName = name if name?
        
        @.eachPolygon line, (err, polygon) ->
          (callback(err); return) if err?

          if progressCb? and not (typeof progressCb is 'undefined')
            progressCb(null, polygon, modelName)

          solidModel.push(polygon)
      , (err) ->
        callback(err, solidModel, modelName)


  # A simple state machine that changes lines to polygons
  #
  # @param - String - line
  # @param - Function - callback
  #
  # callback
  # @param - Error obj - error
  # @param - [ { points: [x, y, z], normal: [a, b, c] }, ... ]
  #
  eachPolygon: (line, callback) ->
    if line.match /facet normal/
      el = line.split(/\s+/).filter (e) -> e.length isnt 0
      @normal = el.slice(2).map (e) -> parseFloat(e)
    else if line.match /vertex/
      el = line.split(/\s+/).filter (e) -> e.length isnt 0
      vertex = el.slice(1).map (e) -> parseFloat(e)
      @verticies.push(vertex)
    else if line.match /endfacet/

      # TODO if there's more than 3 verticies, we want to split them up into
      # multiple polygons.

      callback(null, { verticies: @verticies, normal: @normal })
      @._reset()

  # Helper function that reads each line of the input stream, and yields each line
  # in the callback. It's better to use an input stream, because parsing from a string
  # will block.
  #
  # Assumes the data is ascii string
  # 
  # @param - Stream / String - input
  # @param - function - callback
  #
  # iterator
  # @param - String - line
  #      
  # callback
  # @param - error - error Object
  # 
  #
  eachLine: (input, iterator, callback) ->
    if typeof input is 'string'
      input.split(/\n/).forEach (line) =>
        iterator(line) if iterator?
      callback(null)
    else if Buffer.isBuffer(input)
      console.log "haven't implemented buffer reading of ascii files"
      callback(null)
    else
      remaining = ''

      input.on('data', (data) ->
        remaining += data

        # We're not sure if the data returned in the data buffer ends on an endline,
        # so we only call the iterator when we have complete lines.
        # TODO what's the run time of substring?
        index = remaining.indexOf('\n')
        last = 0
        while (index > -1)
          line = remaining.substring(last, index)
          last = index + 1
          index = remaining.indexOf('\n', last)
          iterator(line)
        remaining = remaining.substring(last, remaining.length)
      )

      input.on('end', () ->
        iterator(remaining) if remaining.length > 0
        callback(null)
      )

      input.on('error', (err) ->
        callback(err)
      )
      input.resume()

class Stl.BinaryParser extends Stl.Parser
  VECTOR_SIZE = 3 * 4

  constructor: () ->

  parse: (input, callback, progressCb) ->
    totalLen = 0
    header = ""
    solidModel = []
    modelName = null
    isFirstData = true

    input.on('data', (data) =>
      totalLen += data.length

      if isFirstData is true and totalLen > 84
        header = data.toString('ascii', 0, 80)
        modelName = header.replace(/\s+/g, '').replace(/\W+/g, '_')
        isFirstData = false
        triangleData = data.slice(84)
      else
        triangleData = data

      @.chunkData(triangleData, 50, (chunkBuf) =>
        normal = @readVector(chunkBuf, 0)
        verticies = [@readVector(chunkBuf, VECTOR_SIZE),
                     @readVector(chunkBuf, 2 * VECTOR_SIZE),
                     @readVector(chunkBuf, 3 * VECTOR_SIZE)]
        attribute = chunkBuf.readUInt16LE(4 * VECTOR_SIZE)

        polygon = { verticies: verticies, normal: normal, attribute: attribute }
        solidModel.push(polygon)

        if progressCb? and not (typeof progressCb is 'undefined')
          progressCb(null, polygon, modelName)
      )
    )

    input.on('end', () =>
      callback(null, solidModel, modelName)
    )

    input.on('error', (err) =>
      callback(err)
    )

    input.resume()

  readVector: (buffer, offset) ->
    x = buffer.readFloatLE(offset)
    y = buffer.readFloatLE(offset + 4)
    z = buffer.readFloatLE(offset + 2 * 4)
    return [x, y, z]

  chunkData: (buffer, numBytes, iterator) ->
    remainingBuf = null

    i = 0
    while i < buffer.length
      if (i + numBytes) <= buffer.length
        chunk = buffer.slice(i, i + numBytes)
      else
        chunk = buffer.slice(i, buffer.length - i)
      iterator(chunk)
      i += numBytes

module.exports = new Stl.Parser()
