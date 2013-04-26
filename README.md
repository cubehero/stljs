# Stljs

A fast library to read and write STL files.

Note: Currently, the readFile works for both binary and ascii files, but writing
STL files is only in ASCII

This currently only works on node.js, but will expand to browser stl parsing soon

## How to install

````
npm install stljs
````

If you'd like to also be able to render STL files, you'll need to install a program
called [pov-ray](http://povray.org). 

If you're on a mac, you can install it using homebrew.

````
brew install povray
````

If you're using ubuntu, you can install it using aptitude

````
aptitude install povray
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

NOTE: In order to use this feature, you need to have pov-ray installed on your system
already. This can be installed separately through 'aptitude' on linux and 'brew'
on macs

## Contributing

Post an issue under 'issues' above, to see if what you want to do isn't already 
being in the works. Then submit a pull request.

## License

MIT license
