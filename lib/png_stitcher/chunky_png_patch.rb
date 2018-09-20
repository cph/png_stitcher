require "chunky_png/chunk"

module ChunkyPNG
  module Chunk
    class Physical < Base

      def ==(other)
        other.is_a?(ChunkyPNG::Chunk::Physical) && self.ppux == other.ppux &&
          self.ppuy == other.ppuy && self.unit == other.unit
      end

    end
  end
end
