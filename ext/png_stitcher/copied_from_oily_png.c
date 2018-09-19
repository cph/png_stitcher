#include "copied_from_oily_png.h"


// https://github.com/wvanbergen/oily_png/blob/v1.2.1/ext/oily_png/png_decoding.c#L10-L15
void oily_png_decode_filter_sub(BYTE* bytes, long pos, long line_length, char pixel_size) {
  long i;
  for (i = 1 + pixel_size; i < line_length; i++) {
    UNFILTER_BYTE(bytes[pos + i], bytes[pos + i - pixel_size]);
  }
}


// https://github.com/wvanbergen/oily_png/blob/v1.2.1/ext/oily_png/png_decoding.c#L18-L26
void oily_png_decode_filter_up(BYTE* bytes, long pos, long line_size, char pixel_size) {
  UNUSED_PARAMETER(pixel_size);
  long i;
  if (pos >= line_size) { // The first line is not filtered because there is no previous line
    for (i = 1; i < line_size; i++) {
      UNFILTER_BYTE(bytes[pos + i], bytes[pos + i - line_size]);
    }
  }
}


// https://github.com/wvanbergen/oily_png/blob/v1.2.1/ext/oily_png/png_decoding.c#L29-L37
void oily_png_decode_filter_average(BYTE* bytes, long pos, long line_size, char pixel_size) {
  long i;
  BYTE a, b;
  for (i = 1; i < line_size; i++) {
    a = (i > pixel_size)     ? bytes[pos + i - pixel_size]  : 0;
    b = (pos >= line_size) ? bytes[pos + i - line_size] : 0;
    UNFILTER_BYTE(bytes[pos + i], (a + b) >> 1);
  }
}


// https://github.com/wvanbergen/oily_png/blob/v1.2.1/ext/oily_png/png_decoding.c#L40-L54
void oily_png_decode_filter_paeth(BYTE* bytes, long pos, long line_size, char pixel_size) {
  BYTE a, b, c, pr;
  long i, p, pa, pb, pc;
  for (i = 1; i < line_size; i++) {
    a = (i > pixel_size) ? bytes[pos + i - pixel_size]  : 0;
    b = (pos >= line_size) ? bytes[pos + i - line_size] : 0;
    c = (pos >= line_size && i > pixel_size) ? bytes[pos + i - line_size - pixel_size] : 0;
    p = a + b - c;
    pa = (p > a) ? p - a : a - p;
    pb = (p > b) ? p - b : b - p;
    pc = (p > c) ? p - c : c - p;
    pr = (pa <= pb) ? (pa <= pc ? a : c) : (pb <= pc ? b : c);
    UNFILTER_BYTE(bytes[pos + i], pr);
  }
}


// https://github.com/wvanbergen/oily_png/blob/v1.2.1/ext/oily_png/png_decoding.c#L61-L65
BYTE oily_png_extract_1bit_element(BYTE* bytes, long start, long x) {
  BYTE byte = bytes[start + 1 + (x >> 3)];
  char bitshift = 7 - (x & (BYTE) 0x07);
  return (byte & (0x01 << bitshift)) >> bitshift;
}
