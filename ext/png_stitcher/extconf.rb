require "mkmf"

# Give it a name
extension_name = 'png_stitcher'

# The destination
dir_config(extension_name)

$CFLAGS << " -Wall"

create_makefile("png_stitcher/png_stitcher")
