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


    static string password_key;
    private:
    //Please change this to a different nonPrime, even number for actual usage
    // (just use evenGenerator but change the bounds so your max wont be absurd and your range will be wide enough)
    static constexpr size_t staticPrivateKey = 1417395678;
    //Formula - sMod(mod(publicKey / privateKey) + mod(publicKey)) + convertPassword();

    struct bounds {
      private:
      //Adjustment of this value is not reccomended
      static constexpr unsigned short multiplier = 2;

      public:
      static constexpr size_t genMin = asymmetric::AKARE::staticPrivateKey*multiplier;
      static constexpr size_t genMax = 99999999999999;
      static constexpr unsigned short expectedLength = 128;
      static constexpr unsigned short bitsInByte = 8;
    };


    //Manually set these to anything between 0 & 65535
    //These function sort of like the static private key, but they just add more randomness
    //to functions which will increase the security of the algorithm even if an attacker has access to
    //the header and algorithm.
    struct entropy {
      public: 
      static constexpr unsigned short Val1 = 10048;
      static constexpr unsigned short Val2 = 65239;
      static constexpr unsigned short Val3 = 21386;
      static constexpr unsigned short Val4 = 5783;
    };

    static void sMod(size_t &inp);
    static size_t mod(size_t inp);
    static size_t convertPassword();

    static bool isDecimal(float inp);
    static bool isValidNum(size_t num);

    static size_t evenGeneratorBackend(size_t min, size_t max, size_t loop=0);

    static size_t evenGenerator(size_t min = bounds::genMin, size_t max = bounds::genMax);

    static void numShuffleAlg(short &inp1, short &inp2, short &inp3, short &inp4, short &inp5, short &inp6, short &inp7, short &inp8, size_t randomseed, bool inverse=false);

    static void numDeShuffleAlg(short &num1, short &num2, short &num3, short &num4, short &num5, short &num6, short &num7, short &num8);

    //Shuffle each key byte's bit
    static string shuffle8(string inp, size_t looper, size_t realKey, bool unShuffle = false);
  };
};