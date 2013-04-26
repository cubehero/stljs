# Stljs

A fast library to read and write STL files.

Note: Currently, the readFile works for both binary and ascii files, but writing
STL files is only in ASCII

This currently only works on node.js, but will expand to browser stl parsing soon

## How to install

````
npm install stljs
````

## Getting started

You have an STL file that you want to read. Here's what you do:

````
stljs = require('stljs')

stljs.readFile('teapot.stl'
  , (err, solid, name) ->
    # can now do stuff with solid
  , (err, polygon, name) ->
    # gets called on as each polygon is parsed
)
````

The readFile takes two functions, a callback when the parsing is done, and an 
optional progress callback, which gets called as each polygon is parsed.

## Converting STL to PNG

Sometimes, you want to convert an STL file into a PNG file. To do that, we can 

````
stljs = require('stljs')

stljs.imageify('teapot.stl', { width: 200, height: 100, dst: 'teapot.png' }
  , (err, povOutput, name) ->
    # done with converting the image
  , (err, povPolygon, name) -> 
    # called on each polygon is processed
)
````

## Contributing

Post an issue under 'issues' above, to see if what you want to do isn't already 
being in the works. Then submit a pull request.

## License

MIT license
