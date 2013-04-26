Mustache = require('mustache')
stl_pov = require('./povray')

fs = require('fs')
path = require('path')
child_proc = require('child_process')

class Stl

class Stl.Image

  constructor: () ->
    @locals = {}
    @filePath = __dirname + "/../../template/povray.tmpl"

  # (input, [options], callback, [progressCb])
  convertFile: (filePath) ->
    options = {
      height: 240
      width: 320
      dst: "./result.png"
    }
    stderrStr = ""

    if typeof arguments[1] is 'function'
      callback = arguments[1]
      progressCb = arguments[2]
    else
      (options[k] = v) for k, v of arguments[1]
      callback = arguments[2]
      progresscb = arguments[3]

    start = convertSTL2POV = () =>
      stl_pov.convertFile filePath
        , (err, povData, name) =>
          (callback(err); return) if err?

          @locals.modelData = povData
          @locals.modelName = name.replace(/\W+/, '_')
          thenInsertIntoTemplate()

        , (err) ->
          (callback(err); return) if err?
          progressCb() if progressCb?

    thenInsertIntoTemplate = () =>
      @renderTemplate(@filePath, (err, povInput) =>
        (callback(err); return) if err?
        thenWriteToTempFile(povInput)
      )
    
    thenWriteToTempFile = (povInput) =>
      fs.writeFile("/tmp/#{@locals.modelName}.pov", povInput, (err) ->
        (callback(err); return) if err?
        thenRenderToImage('temp.pov')
      )

    thenRenderToImage = (povFileName) =>
      povcmd = child_proc.spawn('povray',
        ['-s', "-i/tmp/#{@locals.modelName}.pov", "+FN",
         "+W#{options.width}", "+H#{options.height}",
         "-o#{options.dst}", "+Q9", "+AM1", "+A", "+UA"])

      povcmd.stderr.on('data', (data) ->
        stderrStr += data.toString()
        # console.error data.toString()
      )

      povcmd.on('exit', (code) =>
        if code is 0
          callback(null, stderrStr)
        else
          parsedError = @._parseError(stderrStr)
          if parsedError?
            callback(parsedError, stderrStr)
          else
            callback(Error.new("Exit code: #{code}"), stderrStr)
      )

    start()

  renderTemplate: (filePath, callback) ->
    fs.readFile(filePath, (err, buffer) =>
      (callback(err); return) if err?
      result = Mustache.render(buffer.toString(), @locals)
      callback(null, result)
    )

  _parseError: (stderrstr) ->
    md = stderrstr.match(/(Integer parameter expected.*)/g)
    if (md != null)
      return new Error(md[0])
    else
      return null


module.exports = new Stl.Image()
