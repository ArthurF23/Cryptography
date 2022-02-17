#include "compression_includes.h"

namespace COMPRESSION {
  class binary_compression {
    public:
    static string compress(string path);
    static string decompress(string path);
    private:
    static constexpr int rCharLength[2] = {50, 2};
    static const string replacementChars[rCharLength[0]][rCharLength[1]];

    static constexpr int thirdLayerMultiplier[2] = {10, 2};
    static const string thirdLayerMulRChars[thirdLayerMultiplier[0]][thirdLayerMultiplier[1]];

    static constexpr int fourthLayerMultiplier[2] = {6, 2};
    static const string fourthLayerMulRChars[fourthLayerMultiplier[0]][fourthLayerMultiplier[1]];
    static constexpr char breakChar = ';';
  };
};