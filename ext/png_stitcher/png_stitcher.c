#include "png_stitcher.h"

// Defined below
void encode_scanline_indexed_1bit(BYTE* bytes, BYTE* pixels, long y, long width);
VALUE method_stitch(VALUE self, VALUE datastreams, VALUE width, VALUE height);

// Entry point
void Init_png_stitcher() {
  VALUE PngStitcher = rb_define_module("PngStitcher");
  VALUE PngStitcherExt = rb_define_module_under(PngStitcher, "Ext");
  rb_define_method(PngStitcherExt, "_stitch", method_stitch, 3);
}

// Adapted from OilyPng's methods for decoding and reencoding a PNG.
// See:
//
//   https://github.com/wvanbergen/oily_png/blob/v1.2.1/ext/oily_png/png_decoding.c#L318-L374
//   https://github.com/wvanbergen/oily_png/blob/v1.2.1/ext/oily_png/png_encoding.c#L242-L303
//
VALUE method_stitch(VALUE self, VALUE datastreams, VALUE rb_width, VALUE rb_height) {
  long i;
  long length = RARRAY_LEN(datastreams);
  VALUE datastream, header, imagedata;
  long width = FIX2LONG(rb_width);
  long height = FIX2LONG(rb_height);
  long line_size = (8 + (width + 7)) >> 3;
  long part_height, pass_size, y, line_start, x;
  long p = 0;

  BYTE* pixels = ALLOC_N(BYTE, (width * height));

  for (i = 0; i < length; i++) {
    datastream = rb_ary_entry(datastreams, i);
    header = rb_funcall(datastream, rb_intern("header_chunk"), 0);
    imagedata = rb_funcall(datastream, rb_intern("imagedata"), 0);
    part_height = FIX2LONG(rb_funcall(header, rb_intern("height"), 0));
    pass_size = line_size * part_height;

    // Make sure that the stream is large enough to contain our pass.
    if (RSTRING_LEN(imagedata) < pass_size) {
      rb_raise(rb_eRuntimeError, "The length of the stream is too short to contain the image!");
    }

    // Copy the bytes for this pass from the stream to a separate location
    // so we can work on this byte array directly.
    BYTE* bytes = ALLOC_N(BYTE, pass_size);
    memcpy(bytes, RSTRING_PTR(imagedata), pass_size);

    for (y = 0; y < part_height; y++) {
      line_start = y * line_size;

      // Apply filering to the line
      switch (bytes[line_start]) {
        case OILY_PNG_FILTER_NONE:    break;
        case OILY_PNG_FILTER_SUB:     oily_png_decode_filter_sub(     bytes, line_start, line_size, 1); break;
        case OILY_PNG_FILTER_UP:      oily_png_decode_filter_up(      bytes, line_start, line_size, 1); break;
        case OILY_PNG_FILTER_AVERAGE: oily_png_decode_filter_average( bytes, line_start, line_size, 1); break;
        case OILY_PNG_FILTER_PAETH:   oily_png_decode_filter_paeth(   bytes, line_start, line_size, 1); break;
        default: rb_raise(rb_eRuntimeError, "Filter type not supported: %d", bytes[line_start]);
      }

      // Set the filter byte to 0 because the bytearray is now unfiltered.
      bytes[line_start] = (BYTE) 0;

      for (x = 0; x < width; x++) {
        pixels[p] = oily_png_extract_1bit_element(bytes, line_start, x);
        p++;
      }
    }

    xfree(bytes);
  }

  pass_size = line_size * height;

  // Allocate memory for the byte array.
  BYTE* bytes = ALLOC_N(BYTE, pass_size);

  for (y = height - 1; y >= 0; y--) {
    line_start = y * line_size;
    bytes[line_start] = (BYTE) 0; // no filtering
    encode_scanline_indexed_1bit(bytes + line_start + 1, pixels, y, width);
  }

  xfree(pixels);

  VALUE result = rb_str_new((char*) bytes, pass_size);
  xfree(bytes);
  return result;
}


// https://github.com/wvanbergen/oily_png/blob/v1.2.1/ext/oily_png/png_encoding.c#L144-L157
//
//   MODIFIED to accept `pixels` as a C array of index positions
//   instead of a Ruby array of pixel values.
//
void encode_scanline_indexed_1bit(BYTE* bytes, BYTE* pixels, long y, long width) {
  long x; BYTE p1, p2, p3, p4, p5, p6, p7, p8;
  long pos = y * width;
  for (x = 0; x < width; x += 8) {
    p1 = (x + 0) < width ? pixels[pos + x + 0] : 0;
    p2 = (x + 1) < width ? pixels[pos + x + 1] : 0;
    p3 = (x + 2) < width ? pixels[pos + x + 2] : 0;
    p4 = (x + 3) < width ? pixels[pos + x + 3] : 0;
    p5 = (x + 4) < width ? pixels[pos + x + 4] : 0;
    p6 = (x + 5) < width ? pixels[pos + x + 5] : 0;
    p7 = (x + 6) < width ? pixels[pos + x + 6] : 0;
    p8 = (x + 7) < width ? pixels[pos + x + 7] : 0;
    bytes[x >> 3] = (BYTE) ((p1 << 7) | (p2 << 6) | (p3 << 5) | (p4 << 4) | (p5 << 3) | (p6 << 2) | (p7 << 1) | (p8));
  }
}
