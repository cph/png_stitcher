# PngStitcher

[![Build Status](https://travis-ci.org/cph/png_stitcher.svg)](https://travis-ci.org/cph/png_stitcher)

An optimized way to stack an arbitrary number of PNGs with the same width.

## Installation

Add this line to your application's Gemfile:

```ruby
gem "png_stitcher"
```

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install png_stitcher

## Usage

```ruby
require "png_stitcher"

a = File.binread("/path/to/a.png")
b = File.binread("/path/to/b.png")
c = PngStitcher.stitch([a, b])
File.open("/path/to/c.png", "wb") { |f| f.write(c) }

# c.png now contains a.png + b.png
```

## Development

After checking out the repo, run `bin/setup` to install dependencies. Then, run `rake test` to run the tests. You can also run `bin/console` for an interactive prompt that will allow you to experiment.

To install this gem onto your local machine, run `bundle exec rake install`. To release a new version, update the version number in `version.rb`, and then run `bundle exec rake release`, which will create a git tag for the version, push git commits and tags, and push the `.gem` file to [rubygems.org](https://rubygems.org).

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/cph/png_stitcher.

## License

The gem is available as open source under the terms of the [MIT License](https://opensource.org/licenses/MIT).
