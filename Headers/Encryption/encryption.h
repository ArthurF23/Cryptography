/*
I gave everything default values so it is easier to edit values by
erasing the call to the varible and replacing it with the wanted number
and the defaults are there for when the defaults are wanted again, just type in the varible. Since i used constexpr there shoudlnt be any
slow down or lare memory consumption if my understanting of constexpr
is correct. Please do correct and critique me, i like to learn
*/

//The default key value
#define DEFAULT_KEY_NUM 0;

namespace encryption {
  namespace KEY {
    //This is THE key, made like this so you can just pre set the key and it's used throught the header seamlessly orbyou can prompt the user for a key and generate a key easily or anything with KEY::key
    static unsigned int key = DEFAULT_KEY_NUM;
  }
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
    //Checks to see if the key is valid
    static bool validate_key(string inp);
    //Key generator
    static unsigned int generate_key();
    //Example Function
    static void start_example();
    //Encrypt input string
    static string encrypt(string input);
    //Decrypt input string
    static string decrypt(string input);
    //Part of example, prompts user for key
    static bool get_key(); //True = key valid | False = key invalid
  }; 
}