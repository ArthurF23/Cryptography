#include "compression_includes.h"

namespace COMPRESSION {
  class binary_compression {
    public:
    static void compress(string& input);
    static void decompress(string& input);
    private:
    static constexpr int rCharLength[2] = {50, 2};
    static const string replacementChars[rCharLength[0]][rCharLength[1]];

    static constexpr int thirdLayerMultiplier[2] = {10, 2};
    static const string thirdLayerMulRChars[thirdLayerMultiplier[0]][thirdLayerMultiplier[1]];

    static constexpr int fourthLayerMultiplier[2] = {34, 2};
    static const string fourthLayerMulRChars[fourthLayerMultiplier[0]][fourthLayerMultiplier[1]];

    static constexpr int fifthLayerMultiplier[2] = {6, 2};
    static const string fifthLayerMulRChars[fifthLayerMultiplier[0]][fifthLayerMultiplier[1]];
    static constexpr char breakChar = ';';
  };

  class rgb_compression {
    public:
    //Compress an RGB string.
    static void compress(string &inp, char separator);
    //Decompress a compressed RGB string
    static void decompress(string &inp, char separator);

    static void asgnPix(unsigned char* &pix, string str, char sep, size_t length);

    //private:
    class CORE {
      public:
      class PIXELS {
        public:
        static void asgnPixThr(char* &pix, string data, char sep, uInt startPos, uInt endPos);
        static unsigned int findLength(string str, char sep);
      };
      class COMP {
        public:
        static void compress(string &inp, char separator);
        static constexpr unsigned int sizeLimit = 115100;
      };

      class DECOMP {
        public:
        static void decompress(string &inp, char separator);
        static constexpr short vCharLen = 10;
        static constexpr char validChars[vCharLen] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
        static void errorChecker(string &cln, char sep);
        static bool corruptChunk(string cnk, char sep);
      };

      class FUNC {
        public:
        static string halfify(string &cln, char sep);
        static void replaceSeparator(string &inp, char sep, char repl);
        static void get_chunk_count(unsigned long int &inp, string clone, char separator, bool div = true);
      };

      class VAR {
        public:
        static constexpr char _separator = '#';
        static constexpr char sectionStart = '{';
        static constexpr char sectionEnd = '}';
        static constexpr short firstLayerMultiplier[2] = {80, 2};
        static const string firstLayerMulRChars[firstLayerMultiplier[0]][firstLayerMultiplier[1]];
      };
    };
  };
};