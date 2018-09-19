
lib = File.expand_path("../lib", __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require "png_stitcher/version"

Gem::Specification.new do |spec|
  spec.name          = "png_stitcher"
  spec.version       = PngStitcher::VERSION
  spec.authors       = ["Bob Lail"]
  spec.email         = ["bob.lailfamily@gmail.com"]

  spec.summary       = %q{Stitches an array of PNG blogs into one}
  spec.homepage      = "https://github.com/cph/png_stitcher"
  spec.license       = "MIT"

  spec.files         = `git ls-files -z`.split("\x0").reject do |f|
    f.match(%r{^(test|spec|features)/})
  end
  spec.bindir        = "exe"
  spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.extensions    = ["ext/png_stitcher/extconf.rb"]
  spec.require_paths = ["lib", "ext"]

  spec.add_runtime_dependency "chunky_png"

  spec.add_development_dependency "bundler", "~> 1.16"
  spec.add_development_dependency "rake", "~> 10.0"
  spec.add_development_dependency "rake-compiler"
  spec.add_development_dependency "minitest-reporters-turn_reporter"
  spec.add_development_dependency "minitest-reporters"
  spec.add_development_dependency "minitest", "~> 5.0"
  spec.add_development_dependency "oily_png"
  spec.add_development_dependency "pry"
  spec.add_development_dependency "shoulda-context"
end
