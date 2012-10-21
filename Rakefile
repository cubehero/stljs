task :default => :test

desc "sets up the stuff needed for stl.js"
task :setup do
  IO.popen('cd lib/stl2pov-2.5.0; make') { |f| f.each { |l| puts l } }
end

desc "compiles the stl parser"
task :compile do
  `jison src/stl_parser.json -o lib/jison_stl_parser.js`
end

desc "tests all the code"
task :test do
  IO.popen('node spec/test_read.js') { |f| f.each { |l| puts l } }
  IO.popen('node spec/test_convert.js') { |f| f.each { |l| puts l } }
  puts "tests done!"
end

namespace :benchmark do

  desc "benchmarks between two parsers"
  task :parser do
    puts `node spec/benchmark.js`
    puts "benchmark done!"
  end

end
