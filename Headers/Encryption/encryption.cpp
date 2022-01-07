#include "encryption_includes.h"

#include "encryption.h"

namespace encryption {
  //These will make people think that they have found the pattern when they havent
  string encdec::constants::useless_pattern::random_bs[encdec::constants::useless_pattern::arr_length] = {
          ">._>",
          ">>._",
          ".>_>",
          ">.>>",
          "__>>",
          ">..>",
          "..>.",
          ".>.>",
          ">...",
          ">>__",
          ">.>_",
          "_>..",
          ">.__"
          ">_..",
          ">.>_",
          ">>>>",
          "____",
          "....",
          ">_>>",
          "_.>>"
        };
  void encdec::start_example() {
    cout << "Are you Encrypting or Decrypting?" << endl << "e/d" << endl;
    char user_choice;
    cin >> user_choice;
    cout << "Please input the text" << endl << "Input here:" << endl;
    string user_inputted_string;
    cin.ignore();
    getline(cin >> noskipws, user_inputted_string);
    
    if (user_choice == 'e') {
      KEY::key = encdec::generate_key();
      output_str = encdec::encrypt(user_inputted_string);
    }
    else if (user_choice == 'd') {
      encdec::get_key();
      output_str = encdec::decrypt(user_inputted_string);
      cin.ignore();
    }
    else {
      cout << "ERR please try again" << endl;
      return encdec::start_example();
    };
    cout << "Key: " << KEY::key << endl << "Result: " << endl << output_str << endl;
    cout << "Press any key to continue " << endl;
    cin.ignore();
    return encdec::start_example();
  }
  bool encdec::isNumberString(string input) {
    for (char &c : input) {
      if (!isdigit(c)) {return false;};
    };
    return true;
  }

  int encdec::decide_scramble() {
    for (int i=2; i < 10; i++) {
      if ((KEY::key%i) == 0) {
        return i;
      };
    };
    return 1;
  }

  int encdec::get_random_num(int min, int max) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(min,max);
    return dist6(rng);
  }

  int encdec::generate_key() {
    return encdec::get_random_num(encdec::constants::key_info::key_min_value, encdec::constants::key_info::key_max_value);
  }

  char encdec::generate_ASCII_char() {
    return (char)encdec::get_random_num(encdec::constants::char_info::valid_char_min, encdec::constants::char_info::valid_char_max);
  }

  bool encdec::validate_key(string inp) {
    if (encdec::isNumberString(inp) && inp.length() == encdec::constants::key_info::key_length) {
      return true;
    }
    else {
      return false;
    };
    return false;
  }

  string encdec::get_bloat(int min = encdec::constants::bloat::bloat_min, int max = encdec::constants::bloat::bloat_max) {
    int bloat = encdec::get_random_num(min, max); //How many random characters will bloat the actual message
    string bloat_string;
      for (int x = 0; x < bloat; x++) {
        char bloat_char = encdec::generate_ASCII_char();
        bloat_string += bloat_char;
      }
    return bloat_string;
  }

  string encdec::encrypt(string input) {
    cout << "# " << input << " #" << endl;
    string output;
    int scramble = encdec::decide_scramble();
    //Hide the scramble number in a whole ton of bloat
    output = encdec::get_bloat(10, 20) + to_string(scramble) + encdec::get_bloat(10, 20);
    
    for (int i = 0; i < input.length(); i++) {
      char input_v2 = input[i];
      int input_v3 = input_v2;
      //Magic happens
      //Bloat the string with nonsense to make it harder to decrypt
      output += to_string(input_v3 + (KEY::key/scramble)) + encdec::constants::useless_pattern::random_bs[encdec::get_random_num(0, encdec::constants::useless_pattern::arr_length-1)] + encdec::get_bloat();
      for (int x = 0; x < encdec::get_random_num(encdec::constants::bloat::bloat_repeat_min, encdec::constants::bloat::bloat_repeat_max); x++) {
        output.insert(encdec::get_random_num(0, output.length()), encdec::get_bloat());
      }
    }
    //Add more random patterns to the result to confuse people
    for (int i = 0; i < encdec::get_random_num(encdec::constants::useless_pattern::repeat_min, encdec::constants::useless_pattern::repeat_max) + input.length(); i++) {
      output.insert(encdec::get_random_num(0, output.length()), encdec::constants::useless_pattern::random_bs[encdec::get_random_num(0, encdec::constants::useless_pattern::arr_length-1)]);
    }
    return output;
  }

  string encdec::decrypt(string input) {
    if (KEY::key == 0) {
      return "ERR NO KEY\n Please use encryption::KEY::key = {key} to set the key before usage.";
    };
    string output;
    string filtered;
    for (char &c : input) {
      //remove the bloat from the input string by filtering for only numbers
      if (isdigit(c)) {
        filtered += c;
      };
    };
    int scramble = filtered[0]-'0';
    filtered = filtered.substr(1, filtered.length());

    for (int i = 0; i < filtered.length() - 1; i+=encdec::constants::key_info::key_length) {
      string chunk;
      //substr wouldnt work so heres a loop that grabs 8 characters
      for (int x = 0; x < 8; x++) {
        chunk += filtered[i+x];
      }
      output += char(stoi(chunk) - (KEY::key/scramble));
    }
    return output;
  }

  bool encdec::get_key() {
    cout << "Please input " << encdec::constants::key_info::key_length << " digit numeric key: " << endl;
    string user_input;
    cin >> user_input;
    cout << endl << "# " << user_input << " #" << endl;
    if (encdec::validate_key(user_input)) {
      KEY::key = stoi(user_input);
      return true;
    }
    else {
      cout << "Invalid key, please make sure your key is " << encdec::constants::key_info::key_length << " digits long and does not contain anything besides numbers." << endl;
      return false;
    }
    return false;
  }
}