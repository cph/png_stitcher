$LOAD_PATH.unshift File.expand_path("../../lib", __FILE__)
require "png_stitcher"

# optimizes the tests
require "oily_png"

require "minitest/reporters/turn_reporter"
MiniTest::Reporters.use! Minitest::Reporters::TurnReporter.new

require "shoulda/context"
require "minitest/autorun"
require "pry"
