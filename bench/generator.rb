direc = File.dirname(__FILE__)

require 'rubygems'
#require 'baseline'
require 'benchmark'
require "#{direc}/../lib/perlin"

include Perlin
include Benchmark

SIZE_2D = 500
SIZE_3D = 50

generator = Generator.new 128, 1, 1

puts "\n2D classic Perlin: #{SIZE_2D**2} values"
Benchmark.bm 12 do |x|
  x.report("[]:")      { (SIZE_2D**2).times { generator[0, 0] } }
  x.report("run2d:")   { (SIZE_2D**2).times { generator.run2d 0, 0 } }
  x.report("chunk:")   { generator.chunk 0, 0, SIZE_2D, SIZE_2D }
  x.report("chunk2d:") { generator.chunk2d 0, 0, SIZE_2D, SIZE_2D }
end

puts "\n3D classic Perlin: #{SIZE_3D**3} values"
Benchmark.bm 12 do |x|
  x.report("[]:")      { (SIZE_3D**3).times { generator[0, 0, 0] } }
  x.report("run3d:")   { (SIZE_3D**3).times { generator.run3d 0, 0, 0 } }
  x.report("chunk:")   { generator.chunk 0, 0, 0, SIZE_3D, SIZE_3D, SIZE_3D }
  x.report("chunk3d:") { generator.chunk3d 0, 0, 0, SIZE_3D, SIZE_3D, SIZE_3D }
end