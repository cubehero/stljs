task :default => :test

desc "compiles the stl parser"
task :compile do
  `jison src/stl_parser.json -o lib/jison_stl_parser.js`
end

desc "tests all the code"
task :test do
  puts `node spec/test_read.js`
  puts "tests done!"
end

namespace :benchmark do

  desc "benchmarks between two parsers"
  task :parser do
    puts `node spec/benchmark.js`
    puts "benchmark done!"
  end

end
