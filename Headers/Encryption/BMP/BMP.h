#include "BMP_includes.h"

namespace BITMAP {
  class BMP {
    public:
    static void ReadImage(const char *fileName,BMPbyte **pixels, int32 *width, int32 *height, int32 *bytesPerPixel);
    static void WriteImage(const char *fileName, BMPbyte *pixels, int32 width, int32 height,int32 bytesPerPixel);
  };
}