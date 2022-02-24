/*
I gave everything default values so it is easier to edit values by
erasing the call to the varible and replacing it with the wanted number
and the defaults are there for when the defaults are wanted again, just type in the varible. Since i used constexpr there shoudlnt be any
slow down or lare memory consumption if my understanting of constexpr
is correct. Please do correct and critique me, i like to learn
*/
#include "encryption_includes.h"

constexpr unsigned short GLOBAL_MTX_SIZE = 4*4;

namespace encryption {

  namespace VERSION {
    static string ver = "v1.8.0";
    //Major, Minor, Patch
    //for major or minor, change patch to 0
  }

  namespace KEY {
    //The default key value
    static constexpr unsigned short DEFAULT_KEY_NUM = 0;
    //This is THE key, made like this so you can just pre set the key and it's used throught the header seamlessly orbyou can prompt the user for a key and generate a key easily or anything with KEY::key
    static unsigned int key = DEFAULT_KEY_NUM;
  }
  class VIGENERE_KEY {
    public:
    static string key;
  };
  static string output_str;
  class encdec {
    private:
    //Constants for the encdec header to use
    struct constants {
      public:
      //Info about the expected key. Messing with the key info isnt recommended as these are used as parameters a lot and the program is build around these, and it may cause errors if you modify these values
      struct key_info {
        private:
        struct defaults {
          public:
          //Default minimum value for a key
          static constexpr unsigned int default_key_min_value = 10000000;
          //Default maximum value for a key
          static constexpr unsigned int default_key_max_value = 99999999;
          //Default length for a key
          static constexpr unsigned short default_key_length = 8;
        };
        public:
        //Minimum key value
        static constexpr int key_min_value = encdec::constants::key_info::defaults::default_key_min_value;
        //Maximum key value
        static constexpr int key_max_value = encdec::constants::key_info::defaults::default_key_max_value;
        //Length of a key
        static constexpr short key_length = encdec::constants::key_info::defaults::default_key_length;
      };
      //Info for the range of randomly generated char values used for bloat, the range was selected to not include any numbers which would throw off the decrypion function. Editing these values is not recommended
      struct char_info {
        private:
        struct defaults {
          public:
          //Default minimum char value
          static constexpr unsigned short default_valid_char_min = 58;
          //Default maximum char value
          static constexpr unsigned short default_valid_char_max = 126;
        };
        public:
        //Minimum char value
        static constexpr unsigned short valid_char_min = encdec::constants::char_info::defaults::default_valid_char_min;
        //Maximum char value
        static constexpr unsigned short valid_char_max = encdec::constants::char_info::defaults::default_valid_char_max;
      };
      //Parameters for the bloat function and usage
      struct bloat {
        private:
        struct defaults {
          public:
          //Default minimum value
          static constexpr unsigned short default_bloat_min = 6;
          //Default maximum value
          static constexpr unsigned short default_bloat_max = 10;
          //Default minimum times the function repeats
          static constexpr unsigned short default_bloat_repeat_min = 2;
          //Default maximum times the function repeats
          static constexpr unsigned short default_bloat_repeat_max = 8;
        };
        public:
        //Make the min and max bigger for each to turn up the wack ;)
        static constexpr unsigned short bloat_min = encdec::constants::bloat::defaults::default_bloat_min; //The least amount of characters inserted into the string to bloat it
        static constexpr unsigned short bloat_max = encdec::constants::bloat::defaults::default_bloat_max; //The max amount of characters inserted into the string to bloat it
        static constexpr unsigned short bloat_repeat_min = encdec::constants::bloat::defaults::default_bloat_repeat_min; //The least amount of times the bloat will repeat per letter
        static constexpr unsigned short bloat_repeat_max = encdec::constants::bloat::defaults::default_bloat_repeat_max; //The max amount of times the bloat will repeat per letter
      };
      //This is a pattern that makes it look like there is a pattern going to confuse those trying to decrypt the message
      struct useless_pattern {
        private:
        struct defaults {
          public:
          //Default minimum times a random pattern "chunk" is inserted
          static constexpr unsigned short default_repeat_min = 10;
          //Default maximum times a random pattern "chunk" is inserted
          static constexpr unsigned short default_repeat_max = 20;
          //The "chunks" are stored in a string array, this is the default length of that array
          static constexpr unsigned short default_arr_length = 20;
        };
        public:
        //Minimum times a random "chunk" is inserted
        static constexpr unsigned short repeat_min = encdec::constants::useless_pattern::defaults::default_repeat_min;
        //Maximum times a random "chunk" is inserted
        static constexpr unsigned short repeat_max = encdec::constants::useless_pattern::defaults::default_repeat_max;
        //The array length storing the chunks
        static constexpr unsigned short arr_length = encdec::constants::useless_pattern::defaults::default_arr_length;
        //String definition the the "chunks" are stored in
        static string random_bs[encdec::constants::useless_pattern::arr_length];
      };
      //Parameters for the decide_scramble() function. The parameters are in place as to make sure it is only 1 digit long, if its greater, the decrypting function wont work as it grabs the first digit from the de-bloated string to divide the key, then subtract it from each 8 number chunk
      struct scramble_info {
        private:
        struct defaults {
          public:
          //Default starting divisor
          static constexpr unsigned short default_starting_divisor = 2;
          //Default ending divisor
          static constexpr unsigned short default_ending_divisor = 9;
        };
        public:
        //Starting divisor
        static constexpr unsigned short starting_divisor = encdec::constants::scramble_info::defaults::default_starting_divisor;
        //Ending divisor
        static constexpr unsigned short ending_divisor = encdec::constants::scramble_info::defaults::default_ending_divisor;
      };
    };
    
    //Functions

    //Decides if the key can be evenly divided by a number 2-9 and if it can then this will return that value, if it cant, then it will return 1
    static unsigned int decide_scramble();
    //Random number generation function
    static unsigned int get_random_num(unsigned int min, unsigned int max);
    //Checks to see if an inputted string only contains numbers
    static bool isNumberString(string input);
    //Generates a random ASCII char within the limits of the constant values defined in encdec::constants::char_info
    static char generate_ASCII_char();
    //Gets bloat string, the min and max variables are the minimum and maximum length to be randomly determined in the function, those min and max inputs are also default values so it's able to be called as encdec::get_bloat() or the function can be overloaded
    static string get_bloat(unsigned int min, unsigned int max);

    public:
    enum FLAGS {
      no_bloat = 0b00,
      do_bloat = 0b01,
      no_rand_pattern = 0b10,
      do_rand_pattern = 0b11
    };
    //Checks to see if the key is valid
    static bool assign_key(string inp);
    //Key generator
    static unsigned int generate_key();
    //Encrypt input string
    static string encrypt(string input, FLAGS bloat = FLAGS::do_bloat, FLAGS pattern = FLAGS::do_rand_pattern);
    //Decrypt input string
    static string decrypt(string input);
  }; 

  class AES {
    private:
      static constexpr AESbyte SPACE_BYTE = 0b00000000;
      static constexpr short bitsInByte = 8;
      static constexpr short mtx_size = GLOBAL_MTX_SIZE;
      static constexpr short rounds_of_encryption = 10;  //AES-128 requires 10 rounds of encryption  
      static constexpr short AESwords_in_key = 4;   //Nk Represents the number of AESwords that are input keys
      static constexpr unsigned short expanded_key_size = (4*(rounds_of_encryption+1));

      static constexpr AESbyte S_Box[mtx_size][mtx_size] = {  
        {0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76},  
        {0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0},  
        {0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15},  
        {0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75},  
        {0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84},  
        {0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF},  
        {0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8},  
        {0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2},  
        {0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73},  
        {0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB},  
        {0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79},  
        {0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08},  
        {0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A},  
        {0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E},  
        {0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF},  
        {0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16}  
        };  
      static constexpr AESbyte Inv_S_Box[mtx_size][mtx_size] = {  
        {0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB},  
        {0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB},  
        {0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E},  
        {0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25},  
        {0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92},  
        {0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84},  
        {0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06},  
        {0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B},  
        {0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73},  
        {0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E},  
        {0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B},  
        {0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4},  
        {0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F},  
        {0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF},  
        {0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61},  
        {0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D}  
        };  
      //Round constant, used in key expansion. (AES-128 only takes 10 rounds)
      static constexpr AESword WheelConst[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000,0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000}; 


      static AESword _4Bytes2Word(AESbyte& k1, AESbyte& k2, AESbyte& k3, AESbyte& k4);
      //Position Transformation
      static AESword PosTrans(AESword& rw);
      //S-Box Transformation
      static AESword SBoxTrans(AESword& sw);
      //Helper for SBoxTrans, accepts 4x4 byte matrix
      static void SBoxTransSubBytes(AESbyte mtx[mtx_size]);
      //Line Transformation. Shifts Rows Left
      static void ShiftRows(AESbyte mtx[mtx_size]);
      //Shifts columns
      static void MixColumns(AESbyte mtx[mtx_size]);
      static void AddRoundKey(AESbyte mtx[mtx_size], AESword k[4]);
      static AESbyte GaloisFieldsMul(AESbyte a, AESbyte b); 
      static void InvSBoxTransSubBytes(AESbyte mtx[mtx_size]);
      static void InvShiftRows(AESbyte mtx[mtx_size]);
      static void InvMixColumns(AESbyte mtx[mtx_size]);

      //Conversions
      class CONVERSIONS {
        public:
        static AESbyte char_to_byte_(char inp);
        static char binary_to_char(AESbyte input);
        static AESbyte binStr_to_byte(string input);
      };

      //Random number generation function
      static inline unsigned int getRandomNum(unsigned int min, unsigned int max);
      //Expand Key
      static void KeyExpansion(AESword w[expanded_key_size]);
      //Actual Encrypt function
      static void cypher_encrypt(AESbyte in[mtx_size], AESword w[expanded_key_size]);
      //Actual Decrypt function
      static void cypher_decrypt(AESbyte in[mtx_size], AESword w[expanded_key_size]);

      static AESword global_expanded_key[expanded_key_size]; //Needs to be initilized
      //Generate key
      static void generate_key();

      class FILES {
        public:
        enum CLASSIFIER {
          _RETURN = 0b00000000,
          _TEXT = 0b00000001,
          _BITMAP = 0b00000010
        };
        static const string FILE_EXTENSION;
        static const string KEYFILE_NAME;
        static const string KEYFILE_EXT;
        static constexpr char EXTENSION_SEPERATOR = '~';
        static bool gen_key_file(string path);
        static bool in_key_file(string path);
        static void classify(string ext, CLASSIFIER &clasif);
        
        class TXT {
          public:
          static constexpr short id_len = 6;
          static const string identifier[id_len];
          static void get(string path, string& data);
          static void out(string path, string data);
        };
        class BMP_ {
          public:
          static const string identifier;
          static constexpr char DATA_SEPARATOR = ':';
          static constexpr char NUM_SEPARATOR = ',';
          static void get(string path, string& data);
          static void out(string path, string data);
        };
      };

      public:
      class KEY {
        public:
        static AESbyte key[GLOBAL_MTX_SIZE];
      };
      enum OPTIONS {
        doGenerateKey = 0b00000000, 
        noGenerateKey = 0b00000001,
      };
      //Encrypt
      static string encrypt(string input);
      //Encrypt From File
      static bool encryptFF(string path);
      //Decrypt
      static string decrypt(string input);
      //Decrypt From File
      static bool decryptFF(string path);

      //Call before use
      static void aes_init(OPTIONS genkey, string dummykey = "");

      //Encrypt File 
      static bool encryptFile(string path);
      static bool decryptFile(string path, string keyFilePath = "");

    };

    class DUO {
      public:
      static string encrypt(string input, encryption::encdec::FLAGS bloat = encryption::encdec::FLAGS::do_bloat, encryption::encdec::FLAGS pattern = encryption::encdec::FLAGS::do_rand_pattern);
      static string decrypt(string input);
      static void init(string encdecKey, encryption::AES::OPTIONS genKey = encryption::AES::OPTIONS::doGenerateKey, string aesKey = "");
      static int example();
    };

    class vigenere {
      private:
      static constexpr char table[26][26] = {	{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'}, {'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'A'}, {'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'A', 'B'},{'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'A', 'B', 'C'},{'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'A', 'B', 'C', 'D'},{'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'A', 'B', 'C', 'D', 'E'},{'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'A', 'B', 'C', 'D', 'E', 'F'},{'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'A', 'B', 'C', 'D', 'E', 'F', 'G'},{'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'},{'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I'},{'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'},{'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K'},{'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L'},{'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M'},{'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N'},{'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O'},{'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P'},{'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q'},{'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R'},{'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S'},{'U', 'V', 'W', 'X', 'Y', 'Z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T'},{'V', 'W', 'X', 'Y', 'Z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U'},{'W', 'X', 'Y', 'Z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V'},{'X', 'Y', 'Z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W'},{'Y', 'Z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X'},{'Z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J','K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y'}};
      static constexpr short minGenVal = 65;
      static constexpr short maxGenVal = 90;
      static string ModKey(string inp); //Use to make sure key is correct, if incorrect, the function will make it comply.
      static int translate(char message);
      static int translate(char message, char key);
      static void init(int messageLength); //init for if there is no key used | only used in encrypt()
      static void init(string key); //Init for if there is a pre-exitsing key | only used in decrypt()
      static string generate_key(int message_length);

      public:
      static string encrypt(string inp);
      static string decrypt(string inp, string key);
  };
};