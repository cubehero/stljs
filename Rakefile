task :default => :test

desc "sets up the stuff needed for stl.js"
task :setup do
  IO.popen('cd lib/stl2pov-2.5.0; make') { |f| f.each { |l| puts l } }
end

namespace :compile do
  task :default => :src

  desc "compiles all the coffeescript files into lib"
  task :src do
    IO.popen('coffee -c -o lib/ src/') { |f| f.each { |l| puts l } }
  end

  desc "watches for file changes and compiles"
  task :watch do
    IO.popen('coffee -cw -o lib/ src/') { |f| f.each { |l| puts l } }
  end

  desc "compiles the jison stl parser. Must install jison compiler"
  task :jison do
    `jison src/parser/jison_stl_parser.json -o lib/parser/jison_stl_parser.js`
  end
end

desc "tests all the code"
task :test do
  IO.popen('node spec/test_read.js') { |f| f.each { |l| puts l } }
  IO.popen('node spec/test_convert.js') { |f| f.each { |l| puts l } }
  puts "tests done!"
end

desc "runs the profiler on file"
task "profile" do
  if ARGV[1].nil?
    puts "need a path to a file"
    exit
  end
  IO.popen("coffee --nodejs --prof #{ARGV[1]}; mac-tick-processor v8.log") { |f|
    f.each { |l| puts l }
  }
end

namespace :benchmark do

  desc "benchmarks between two parsers"
  task :parser do
    puts `node spec/benchmark.js`
    puts "benchmark done!"
  end

end
