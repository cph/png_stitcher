require "test_helper"

class PngStitcherTest < Minitest::Test

  context "stitch" do
    should "return an identical image " do
      image = File.binread(fixture_file("Fa.png"))
      actual = PngStitcher.stitch([ image ])
      assert_equal ChunkyPNG::Canvas.from_blob(image), ChunkyPNG::Canvas.from_blob(actual)
    end

    should "concatenate several PNGs together" do
      files = [
        File.binread(fixture_file("0.png")),
        File.binread(fixture_file("1.png")) ]

      expected = ChunkyPNG::Canvas.from_file(fixture_file("01.png"))
      actual = PngStitcher.stitch(files)
      assert_equal expected, ChunkyPNG::Canvas.from_blob(actual)
    end
  end

private

  def fixture_file(name)
    File.join(File.dirname(__FILE__), "fixtures", name)
  end

end
