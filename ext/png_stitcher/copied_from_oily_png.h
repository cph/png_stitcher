#ifndef PNG_STITCHER_COPIED_FROM_OILY_PNG_EXT
#define PNG_STITCHER_COPIED_FROM_OILY_PNG_EXT

#define RSTRING_NOT_MODIFIED

// PNG filter constants
#define OILY_PNG_FILTER_NONE    0
#define OILY_PNG_FILTER_SUB     1
#define OILY_PNG_FILTER_UP      2
#define OILY_PNG_FILTER_AVERAGE 3
#define OILY_PNG_FILTER_PAETH   4

// Macro to suppress warnings about unused parameters.
#define UNUSED_PARAMETER(param) (void) param

//
#define UNFILTER_BYTE(byte, adjustment)  byte = (BYTE) (((byte) + (adjustment)) & 0x000000ff)

// Type definitions
typedef unsigned char BYTE; // Bytes use 8 bits unsigned integers

void oily_png_decode_filter_sub(BYTE* bytes, long pos, long line_length, char pixel_size);
void oily_png_decode_filter_up(BYTE* bytes, long pos, long line_size, char pixel_size);
void oily_png_decode_filter_average(BYTE* bytes, long pos, long line_size, char pixel_size);
void oily_png_decode_filter_paeth(BYTE* bytes, long pos, long line_size, char pixel_size);

BYTE oily_png_extract_1bit_element(BYTE* bytes, long start, long x);

#endif
