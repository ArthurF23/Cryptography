namespace encryption {
  namespace KEY {
    static int key = 0;
  }
  static string output_str;
  class encdec {
    private:
    struct constants {
      public:
      struct key_info {
        private:
        struct defaults {
          public:
          static constexpr unsigned int default_key_min_value = 10000000;
          static constexpr unsigned int default_key_max_value = 99999999;
          static constexpr unsigned short default_key_length = 8;
        };
        public:
        //This is the info for key generation and usage. Dont mess with these as it will change a lot of things.
        static constexpr int key_min_value = encdec::constants::key_info::defaults::default_key_min_value;
        static constexpr int key_max_value = encdec::constants::key_info::defaults::default_key_max_value;
        static constexpr short key_length = encdec::constants::key_info::defaults::default_key_length;
      };      
      struct char_info {
        private:
        struct defaults {
          public:
          static constexpr short default_valid_char_min = 58;
          static constexpr short default_valid_char_max = 126;
        };
        public:
        //Ascii range that wont include numbers but has enough random characters to bloat the message. Don't edit these values
        static constexpr short valid_char_min = encdec::constants::char_info::defaults::default_valid_char_min;
        static constexpr short valid_char_max = encdec::constants::char_info::defaults::default_valid_char_max;
      };
      struct bloat {
        private:
        struct defaults {
          public:
          static constexpr short default_bloat_min = 6;
          static constexpr short default_bloat_max = 10;
          static constexpr short default_bloat_repeat_min = 2;
          static constexpr short default_bloat_repeat_max = 8;
        };
        public:
        //Make the min and max bigger for each to turn up the wack ;)
        static constexpr short bloat_min = encdec::constants::bloat::defaults::default_bloat_min; //The least amount of characters inserted into the string to bloat it
        static constexpr short bloat_max = encdec::constants::bloat::defaults::default_bloat_max; //The max amount of characters inserted into the string to bloat it
        static constexpr short bloat_repeat_min = encdec::constants::bloat::defaults::default_bloat_repeat_min; //The least amount of times the bloat will repeat per letter
        static constexpr short bloat_repeat_max = encdec::constants::bloat::defaults::default_bloat_repeat_max; //The max amount of times the bloat will repeat per letter
      };
      struct useless_pattern {
        private:
        struct defaults {
          public:
          static constexpr short default_repeat_min = 10;
          static constexpr short default_repeat_max = 20;
        static constexpr short default_arr_length = 20;
        };
        public:
        static constexpr short repeat_min = encdec::constants::useless_pattern::defaults::default_repeat_min;
        static constexpr short repeat_max = encdec::constants::useless_pattern::defaults::default_repeat_max;
        static constexpr short arr_length = encdec::constants::useless_pattern::defaults::default_arr_length;
        static string random_bs[encdec::constants::useless_pattern::arr_length];
      };
    };

    static int decide_scramble();
    static int get_random_num(int min, int max);
    static bool isNumberString(string input);
    static int generate_key();
    static char generate_ASCII_char();
    static string get_bloat(int min, int max);
    static bool validate_key(string inp);

    public:
    static void start_example(); //Example function
    static string encrypt(string input);
    static string decrypt(string input);
    static bool get_key(); //True = key valid | False = key invalid
  }; 
}