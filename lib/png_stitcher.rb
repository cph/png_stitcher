require "png_stitcher/version"
require "png_stitcher/png_stitcher"
require "chunky_png"

module PngStitcher
  class << self
    include PngStitcher::Ext

    def stitch(blobs)
      streams = []
      height = 0
      width = nil
      palette = nil

      blobs.each do |blob|
        stream = ChunkyPNG::Datastream.from_blob(blob)
        palette ||= ChunkyPNG::Palette.from_chunks(stream.palette_chunk, stream.transparency_chunk)
        header = stream.header_chunk
        width ||= header.width
        height += header.height

        raise ArgumentError, "Color Mode must be Indexed" unless header.color == ChunkyPNG::COLOR_INDEXED
        raise ArgumentError, "Pixel Depth must be 1 bit per pixel" unless header.depth == 1
        raise ArgumentError, "Interlacing must be disabled" unless header.interlace == ChunkyPNG::INTERLACING_NONE
        raise ArgumentError, "All of the images must be the same width" unless header.width == width
        raise ArgumentError, "All of the images must have the same palette" unless palette == ChunkyPNG::Palette.from_chunks(stream.palette_chunk, stream.transparency_chunk)

        streams << stream
      end

      data = _stitch(streams, width, height)

      stream = ChunkyPNG::Datastream.new
      stream.header_chunk = ChunkyPNG::Chunk::Header.new(
        width: width,
        height: height,
        color: ChunkyPNG::COLOR_INDEXED,
        depth: 1,
        interlace: 0)
      stream.palette_chunk      = palette.to_plte_chunk
      stream.transparency_chunk = palette.to_trns_chunk unless palette.opaque?

      stream.data_chunks = ChunkyPNG::Chunk::ImageData.split_in_chunks(data)
      stream.end_chunk = ChunkyPNG::Chunk::End.new
      stream.to_blob
    end

  end
end
