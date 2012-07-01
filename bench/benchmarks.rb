require 'rubygems'
require 'benchmark'
require File.expand_path("../../lib/perlin", __FILE__)

SIZE_2D = 500
SIZE_3D = 50

t = Time.now

generator = Perlin::Generator.new 128, 1, 1

puts "\n2D classic Perlin: #{SIZE_2D**2} values"
Benchmark.bm 12 do |x|
  x.report("[]:")         { (SIZE_2D**2).times { generator[0, 0] } }
  x.report("run2d:")      { (SIZE_2D**2).times { generator.run2d 0, 0 } }
  x.report("chunk:")      { generator.chunk(0, 0, SIZE_2D, SIZE_2D).each {|r| r.each {|h| } } }
  x.report("chunk {}:")   { generator.chunk(0, 0, SIZE_2D, SIZE_2D) {|h, x, y| } }
  x.report("chunk2d:")    { generator.chunk2d(0, 0, SIZE_2D, SIZE_2D).each {|r| r.each {|h| } } }
  x.report("chunk2d {}:") { generator.chunk2d(0, 0, SIZE_2D, SIZE_2D) {|h, x, y| } }
end

puts "\n3D classic Perlin: #{SIZE_3D**3} values"
Benchmark.bm 12 do |x|
  x.report("[]:")         { (SIZE_3D**3).times { generator[0, 0, 0] } }
  x.report("run3d:")      { (SIZE_3D**3).times { generator.run3d 0, 0, 0 } }
  x.report("chunk:")      { generator.chunk(0, 0, 0, SIZE_3D, SIZE_3D, SIZE_3D).each {|r| r.each {|c| c.each {|h| } } } }
  x.report("chunk {}:")   { generator.chunk(0, 0, 0, SIZE_3D, SIZE_3D, SIZE_3D) {|h, x, y, z| } }
  x.report("chunk3d:")    { generator.chunk3d(0, 0, 0, SIZE_3D, SIZE_3D, SIZE_3D).each {|r| r.each {|c| c.each {|h| } } } }
  x.report("chunk3d {}:") { generator.chunk3d(0, 0, 0, SIZE_3D, SIZE_3D, SIZE_3D) {|h, x, y, z| } }
end

puts "\n\nBenchmarks completed in #{"%.3f" % (Time.now - t)} s"