require('./csg');

/* Handcoded parser for ASCII STL files.
 * Originally created a parser from jison, but it was running too slow, and half the time
 * spent calculating the diff was spent on parsing the STL file. Hnadcoded parser is an 
 * order of magnitude faster, and this is possible because STL is pretty simple format
 */
var STLParser = function() {
  this._reset();
};

/* Helper function that reads each line of the input stream, and yields each line
 * in the callback.
 *
 * @param - Stream / String - input
 * @param - function - callback
 *
 * progessCallback
 * @param - String - line
 *
 * doneCallback
 * @param - error - error Object
 */
STLParser.eachLine = function(input, progressCallback, doneCallback) {
  if (typeof input === 'string') {
    input.split(/\n/).forEach(function(line) {
      progressCallback(line);
    }, this);
    doneCallback(null);
  } else if (typeof input === 'object' && input.readable !== null) {
    var remaining = '';

    input.on('data', function(data) {
      remaining = remaining.concat(data);

      var index = remaining.indexOf('\n');
      var last = 0;
      while (index > -1) {
        var line = remaining.substring(last, index);
        last = index + 1;
        index = remaining.indexOf('\n', last);
        progressCallback(line);
      }
      remaining = remaining.substring(last, remaining.length);
    });

    input.on('end', function() {
      if (remaining.length > 0) progressCallback(remaining);
      if (typeof doneCallback !== 'undefined' && doneCallback !== null)
        doneCallback(null);
    });
  }
}

STLParser.prototype = {

  /* parses an ACSII STL file
   *
   * @param - Stream / String - input
   * @param - function - callback
   *
   * progressCallback
   * @param - error - error object
   * @param - CSG.polygon - polygon object
   *
   * doneCallback
   * @param - error - error Object
   * @params - Array - a list of polygons
   */
  parse: function(input, progressCallback, doneCallback) {
    var that = this;
    // FIXME have an option to say whether we want an accumulator, 
    // since it can take up quite a bit of memroy.
    var solid = [];

    STLParser.eachLine(input, function(line) {
      that.eachPolygon(line, function(err, polygon) {
        if (typeof progressCallback !== 'undefined' && progressCallback !== null)
          progressCallback(err, polygon);
        solid.push(polygon);
      });
    }, function(err) {
      if (typeof doneCallback !== 'undefined' && doneCallback !== null)
        doneCallback(err, solid);
    });
  },

  _reset: function() {
    this.normal = [];
    this.verticies = [];
  },

  /* A simple state machine that changes lines to polygons
   *
   * @param - String - line
   * @param - Function - callback
   *
   * callback
   * @param - Error obj - error
   * @param - CSG.polygon - polygon
   */
  eachPolygon: function(line, callback) {
    if (line.match(/facet normal/)) {
      var el = line.split(/\s+/).filter(function(e) { return e.length != 0 });
      this.normal = el.slice(2);
      this.normal = this.normal.map(function(e) { return parseFloat(e) });
    } else if (line.match(/vertex/)) {
      var el = line.split(/\s+/).filter(function(e) { return e.length != 0 });
      var points = el.slice(1);
      points = points.map(function(e) { return parseFloat(e) });
      this.verticies.push(new CSG.Vertex(points, this.normal));
    } else if (line.match(/endfacet/)) {
      callback(null, new CSG.Polygon(this.verticies));
      this._reset();
    }
  },

}

module.exports = new STLParser();

