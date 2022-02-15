#include "compression_includes.h"

namespace COMPRESSION {
  class compression {
    public:
    static constexpr char occSeparator = ',', separator1 = ':', startPosSeparator = '{', stopPosSeparator = '}';
    static string compress(string path);
    static string decompress(string path);
    

    private:
    static string findOccOf(char search, string str);

    // represents a value that will never be read;
    static const int NULL_VALUE = -1;    
  };
};