#include "compression_includes.h"

namespace COMPRESSION {
  class binary_compression {
    public:
    static string compress(string path);
    static string decompress(string path);
    private:
    static constexpr int rCharLength[2] = {20, 2};
    static const string replacementChars[rCharLength[0]][rCharLength[1]];
    static constexpr char breakChar = ';';
  };
};