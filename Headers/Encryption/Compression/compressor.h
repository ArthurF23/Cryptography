#include "compression_includes.h"

namespace COMPRESSION {
  class binary_compression {
    public:
    static string compress(string path);
    static string decompress(string path);
    private:
    static constexpr char breakChar = ';';
  };
};