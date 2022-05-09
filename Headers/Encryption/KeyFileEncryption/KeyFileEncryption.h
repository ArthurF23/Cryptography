#include "KeyFileEncryption_Includes.h"

namespace asymmetric {
  //Asymmetric
  //Key File
  //Abridged
  //RSA Ripoff
  //Encryption
  class AKARE {
    public:
    static string encrypt(string inp, size_t &key);
    static string decrypt(string inp, size_t key);

    //private:
    //Please change this to a different nonPrime, even number for actual usage
    // (just use evenGenerator but change the bounds so your max wont be absurd and your range will be wide enough)
    static constexpr size_t staticPrivateKey = 1417395678;
    //Formula - sMod(mod(publicKey / privateKey) + mod(publicKey))

    struct bounds {
      public:
      static constexpr size_t genMin = asymmetric::AKARE::staticPrivateKey*2;
      static constexpr size_t genMax = 99999999999999;
      static constexpr unsigned short expectedLength = 128;
      static constexpr unsigned short bitsInByte = 8;
    };

    static void sMod(size_t &inp);
    static size_t mod(size_t inp);

    static bool isDecimal(float inp);
    static bool isValidNum(size_t num);

    static size_t evenGeneratorBackend(size_t min, size_t max, size_t loop=0);

    static size_t evenGenerator(size_t min = bounds::genMin, size_t max = bounds::genMax);

    //Shuffle each key byte's bit
    static string shuffle8(string inp, size_t looper);
    //Unshuffle - Decryption
    static string unshuffle8(string inp, size_t looper);
  };
};