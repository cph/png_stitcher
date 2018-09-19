require "bundler/gem_tasks"
require "rake/testtask"
require "rake/extensiontask"

Rake::ExtensionTask.new("png_stitcher") do |ext|
  ext.lib_dir = File.join("lib", "png_stitcher")
  ext.config_options = '--with-cflags="-std=c99"'
end

Rake::TestTask.new(:test) do |t|
  t.libs << "test"
  t.libs << "lib"
  t.test_files = FileList["test/**/*_test.rb"]
end

Rake::Task[:test].prerequisites << :compile

task :default => :test
