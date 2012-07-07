require 'rake/clean'
require 'rspec/core/rake_task'
require 'rake/extensiontask'
require 'yard'
require 'redcloth'
require 'launchy'


begin
  require 'devkit' # only used on windows
rescue LoadError
end

require File.expand_path('../lib/perlin/version', __FILE__)

CLOBBER << "coverage"

# somewhere in your Rakefile, define your gem spec
spec = Gem::Specification.new do |s|
  s.name = 'perlin'
  s.version = Perlin::VERSION
  s.date = Time.now.strftime '%Y-%m-%d'
  s.authors = ["Brian 'bojo' Jones", 'Camille Goudeseune', 'Bil Bas']

  s.summary = 'Perlin Noise C extension'
  s.description = <<-END
#{s.summary}

A Perlin/Simplex noise implementation based of
<http://freespace.virgin.net/hugo.elias/models/m_perlin.htm>. Implemented as a Ruby C extension.
  END

  s.email = %w<mojobojo@gmail.com>
  s.files = Dir.glob %w<CHANGELOG LICENSE Rakefile README.md lib/**/*.* lib ext/**/*.* examples/**/*.*>
  s.homepage = 'https://github.com/boj/ruby-perlin'
  s.licenses = %w<MIT>
  s.extensions << 'ext/perlin/extconf.rb'
  s.rubyforge_project = 'ruby-perlin'
  s.test_files = []
  s.has_rdoc = 'yard'

  s.add_development_dependency 'rake-compile', '~> 0.8.1'
  s.add_development_dependency 'simplecov', '~> 0.6.4'
  s.add_development_dependency 'launchy', '~> 2.1.0'
end

Gem::PackageTask.new spec do
end

Rake::ExtensionTask.new 'perlin', spec do |ext|
  RUBY_VERSION =~ /(\d+.\d+)/
  ext.lib_dir = "lib/perlin/#{$1}"
end

YARD::Rake::YardocTask.new

task :default => :spec
task :spec => :compile

RSpec::Core::RakeTask.new do |t|
end

desc "Generate SimpleCov test coverage and open in your browser"
task :coverage do
  rm_r "coverage" rescue nil

  sh %q<ruby -rsimplecov -e "SimpleCov.command_name 'spec'; SimpleCov.start">

  Launchy.open "coverage/index.html" rescue nil
end

desc "Open yard docs in browser"
task :browse_yard => :yard do
  Launchy.open "doc/index.html" rescue nil
end

desc "Create platform-specific compiled gem"
task :native_gem do
  system "rake native gem"
end

desc "Run benchmarks"
task :bench => :compile do
  require File.expand_path("../bench/benchmarks.rb", __FILE__)
end