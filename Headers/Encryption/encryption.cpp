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
    char user_choice; cin >> user_choice; cout << "Please input the text" << endl << "Input here:" << endl; string user_inputted_string;
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

  unsigned int encdec::decide_scramble() {
    for (int i=encdec::constants::scramble_info::starting_divisor; i < encdec::constants::scramble_info::ending_divisor+1; i++) {
      if ((KEY::key%i) == 0) {
        return i;
      };
    };
    return 1;
  }

  unsigned int encdec::get_random_num(unsigned int min, unsigned int max) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(min,max);
    return dist6(rng);
  }

  unsigned int encdec::generate_key() {
    return encdec::get_random_num(encdec::constants::key_info::key_min_value, encdec::constants::key_info::key_max_value);
  }

  char encdec::generate_ASCII_char() {
    return (char)encdec::get_random_num(encdec::constants::char_info::valid_char_min, encdec::constants::char_info::valid_char_max);
  }

  bool encdec::validate_key(string inp) {
    if (encdec::isNumberString(inp)) {
      if (inp.length() == encdec::constants::key_info::key_length && stoi(inp) >= encdec::constants::key_info::key_min_value) {
        return true;
      };
    }
    else {
      return false;
    };
    return false;
  }

  string encdec::get_bloat(unsigned int min = encdec::constants::bloat::bloat_min, unsigned int max = encdec::constants::bloat::bloat_max) {
    int bloat = encdec::get_random_num(min, max); //How many random characters will bloat the actual message
    string bloat_string;
      for (int x = 0; x < bloat; x++) {
        char bloat_char = encdec::generate_ASCII_char();
        bloat_string += bloat_char;
      }
    return bloat_string;
  }

  string encdec::encrypt(string input, FLAGS bloat, FLAGS pattern) {
    cout << "# " << input << " #" << endl;
    string output;
    int scramble = encdec::decide_scramble();
    //Hide the scramble number in a whole ton of bloat
    output = (bloat == FLAGS::do_bloat ? encdec::get_bloat(10, 20) : "") + to_string(scramble) + (bloat == FLAGS::do_bloat ? encdec::get_bloat(10, 20) : "");
    
    for (int i = 0; i < input.length(); i++) {
      char input_v2 = input[i];
      int input_v3 = input_v2;
      //Magic happens
      //Bloat the string with nonsense to make it harder to decrypt
      output += to_string(input_v3 + (KEY::key/scramble)) + (pattern == FLAGS::do_rand_pattern ? encdec::constants::useless_pattern::random_bs[encdec::get_random_num(0, encdec::constants::useless_pattern::arr_length-1)] : "") + (bloat == FLAGS::do_bloat ? encdec::get_bloat() : "");
      if (bloat == FLAGS::do_bloat) {
        for (int x = 0; x < encdec::get_random_num(encdec::constants::bloat::bloat_repeat_min, encdec::constants::bloat::bloat_repeat_max); x++) {
          output.insert(encdec::get_random_num(0, output.length()), encdec::get_bloat());
        };
      };
    };
    //Add more random patterns to the result to confuse those trying to decrypt the message
    if (pattern == FLAGS::do_rand_pattern) {
      for (int i = 0; i < encdec::get_random_num(encdec::constants::useless_pattern::repeat_min, encdec::constants::useless_pattern::repeat_max) + input.length(); i++) {
        output.insert(encdec::get_random_num(0, output.length()), encdec::constants::useless_pattern::random_bs[encdec::get_random_num(0, encdec::constants::useless_pattern::arr_length-1)]);
      };
    };
    return output;
  }

  string encdec::decrypt(string input) {
    if (KEY::key == KEY::DEFAULT_KEY_NUM) {
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
    
    const unsigned short scramble = filtered[0]-'0';
    filtered = filtered.substr(1, filtered.length());

    for (int i = 0; i < filtered.length() - 1; i+=encdec::constants::key_info::key_length) {
      string chunk;
      //substr wouldnt work so heres a loop that grabs 8 characters
      for (int x = 0; x < 8; x++) {
        chunk += filtered[i+x];
      }
      output += char(stoi(chunk) - (KEY::key/scramble));
    };
    return output;
  }

  bool encdec::get_key() {
    cout << "Please input " << encdec::constants::key_info::key_length << " digit numeric key: " << endl;
    string user_input; cin >> user_input; cout << endl << "# " << user_input << " #" << endl;
    if (encdec::validate_key(user_input)) {
      KEY::key = stoi(user_input);
      return true;
    }
    else {
      cout << "Invalid key, please make sure your key is " << encdec::constants::key_info::key_length << " digits long and does not contain anything besides numbers." << endl;
      return false;
    }
    return false;
  };
  
  /////////////////////////////////////////////////////////
  ///////// AES ///////////////////////////////////////////
  /////////////////////////////////////////////////////////

  AESbyte AES::SPACE[mtx_size] = {SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE};

  AESword AES::Word(AESbyte& k1, AESbyte& k2, AESbyte& k3, AESbyte& k4) {
    AESword result(0x00000000);  
    AESword temp;  
    temp = k1.to_ulong();  // K1  
    temp <<= 24;  
    result |= temp;  
    temp = k2.to_ulong();  // K2  
    temp <<= 16;  
    result |= temp;  
    temp = k3.to_ulong();  // K3  
    temp <<= 8;  
    result |= temp;  
    temp = k4.to_ulong();  // K4  
    result |= temp;  
    return result;  
  };

  AESword AES::RotWord(AESword& rw) {
    AESword high = rw << 8;  
    AESword low = rw >> 24;  
    return high | low;  
  };

  AESword AES::SubWord(AESword& sw)  {  
    AESword temp;  
    for(int i=0; i<32; i+=8)  {  
        int row = sw[i+7]*8 + sw[i+6]*4 + sw[i+5]*2 + sw[i+4];  
        int col = sw[i+3]*8 + sw[i+2]*4 + sw[i+1]*2 + sw[i];  
        AESbyte val = AES::S_Box[row][col];  
        for(int j=0; j<8; ++j)  
            temp[i+j] = val[j];  
    };  
    return temp;  
  };

  void AES::KeyExpansion(AESword w[word_size]) {  
    AESword temp;
    int i = 0;  
    //The first four of w [] are input keys  
    while(i < Nk) {  
        w[i] = Word(AESKEY::key[4*i], AESKEY::key[4*i+1], AESKEY::key[4*i+2], AESKEY::key[4*i+3]);  
        ++i;  
    };
  
    i = Nk;  
  
    while(i < word_size) {  
      temp = w[i-1]; //Record the previous word  
      if(i % Nk == 0) {
        AESword rwt = RotWord(temp);
        w[i] = w[i-Nk] ^ SubWord(rwt) ^ Rcon[i/Nk-1];  
      }
      else {
        w[i] = w[i-Nk] ^ temp;  
      };
      ++i;  
    };
  };

  void AES::SubBytes(AESbyte mtx[mtx_size]) {  
    for(int i=0; i<mtx_size; ++i)  {  
        int row = mtx[i][7]*8 + mtx[i][6]*4 + mtx[i][5]*2 + mtx[i][4];  
        int col = mtx[i][3]*8 + mtx[i][2]*4 + mtx[i][1]*2 + mtx[i][0];  
        mtx[i] = S_Box[row][col];  
    };
  };

  void AES::ShiftRows(AESbyte mtx[mtx_size]) {  
    //The second line circle moves one bit to the left  
    AESbyte temp = mtx[4];  
    for(int i=0; i<3; ++i) {
      mtx[i+4] = mtx[i+5];
    };

    mtx[7] = temp;

    //The third line circle moves two places to the left  
    for(int i=0; i<2; ++i) {  
        temp = mtx[i+8];  
        mtx[i+8] = mtx[i+10];  
        mtx[i+10] = temp;  
    };

    //The fourth line moves three left circles  
    temp = mtx[15];  
    for(int i=3; i>0; --i)  
        mtx[i+12] = mtx[i+11];  
    mtx[12] = temp;
  };  
  
  //Multiplication over Finite Fields GF(2^8) 
    
  AESbyte AES::GFMul(AESbyte a, AESbyte b) {   
      AESbyte p = 0;  
      AESbyte hi_bit_set;  
      for (int counter = 0; counter < 8; counter++) {  
          if ((b & AESbyte(1)) != 0) {  
              p ^= a;  
          }  
          hi_bit_set = (AESbyte) (a & AESbyte(0x80));  
          a <<= 1;  
          if (hi_bit_set != 0) {  
              a ^= 0x1b; /* x^8 + x^4 + x^3 + x + 1 */  
          }  
          b >>= 1;  
      }  
      return p;  
  };

  void AES::MixColumns(AESbyte mtx[mtx_size])  {  
    AESbyte arr[4];  
    for(int i=0; i<4; ++i)  {  
        for(int j=0; j<4; ++j) {
          arr[j] = mtx[i+j*4];  
        };  
        mtx[i] = GFMul(0x02, arr[0]) ^ GFMul(0x03, arr[1]) ^ arr[2] ^ arr[3];  
        mtx[i+4] = arr[0] ^ GFMul(0x02, arr[1]) ^ GFMul(0x03, arr[2]) ^ arr[3];  
        mtx[i+8] = arr[0] ^ arr[1] ^ GFMul(0x02, arr[2]) ^ GFMul(0x03, arr[3]);  
        mtx[i+12] = GFMul(0x03, arr[0]) ^ arr[1] ^ arr[2] ^ GFMul(0x02, arr[3]);  
    }; 
  };

  void AES::AddRoundKey(AESbyte mtx[mtx_size], AESword k[4]) {  
    for(int i=0; i<4; ++i) {  
      AESword k1 = k[i] >> 24;  
      AESword k2 = (k[i] << 8) >> 24;  
      AESword k3 = (k[i] << 16) >> 24;  
      AESword k4 = (k[i] << 24) >> 24;  
        
      mtx[i] = mtx[i] ^ AESbyte(k1.to_ulong());  
      mtx[i+4] = mtx[i+4] ^ AESbyte(k2.to_ulong());  
      mtx[i+8] = mtx[i+8] ^ AESbyte(k3.to_ulong());  
      mtx[i+12] = mtx[i+12] ^ AESbyte(k4.to_ulong());  
    };  
  };

  void AES::InvSubBytes(AESbyte mtx[mtx_size]) {  
    for(int i=0; i<mtx_size; ++i) {  
      int row = mtx[i][7]*8 + mtx[i][6]*4 + mtx[i][5]*2 + mtx[i][4];  
      int col = mtx[i][3]*8 + mtx[i][2]*4 + mtx[i][1]*2 + mtx[i][0];  
      mtx[i] = Inv_S_Box[row][col];
    };
  };

  void AES::InvShiftRows(AESbyte mtx[mtx_size]) {  
    //The second line circle moves one bit to the right  
    AESbyte temp = mtx[7];  
    for(int i=3; i>0; --i)  
        mtx[i+4] = mtx[i+3];  
    mtx[4] = temp;  
    //The third line circle moves two to the right  
    for(int i=0; i<2; ++i) {  
        temp = mtx[i+8];  
        mtx[i+8] = mtx[i+10];  
        mtx[i+10] = temp;  
    };
    //Fourth line circle moves three to the right  
    temp = mtx[12];  
    for(int i=0; i<3; ++i)  
        mtx[i+12] = mtx[i+13];  
    mtx[15] = temp;  
  };  

  void AES::InvMixColumns(AESbyte mtx[mtx_size]) {  
    AESbyte arr[4];  
    for(int i=0; i<4; ++i) {  
        for(int j=0; j<4; ++j) {
          arr[j] = mtx[i+j*4];  
        };  
        mtx[i] = GFMul(0x0e, arr[0]) ^ GFMul(0x0b, arr[1]) ^ GFMul(0x0d, arr[2]) ^ GFMul(0x09, arr[3]);  
        mtx[i+4] = GFMul(0x09, arr[0]) ^ GFMul(0x0e, arr[1]) ^ GFMul(0x0b, arr[2]) ^ GFMul(0x0d, arr[3]);  
        mtx[i+8] = GFMul(0x0d, arr[0]) ^ GFMul(0x09, arr[1]) ^ GFMul(0x0e, arr[2]) ^ GFMul(0x0b, arr[3]);  
        mtx[i+12] = GFMul(0x0b, arr[0]) ^ GFMul(0x0d, arr[1]) ^ GFMul(0x09, arr[2]) ^ GFMul(0x0e, arr[3]);  
    };
  };

  string AES::hex_to_string(AESbyte inp[mtx_size]) {
    string output;
    for (int i=0; i < mtx_size; i++) {
      output += static_cast<char>(inp[i].to_ulong());
    };
    return output;
  }

  void AES::generate_key() {
    
  };

  //Basically converting char to hex
  AESbyte AES::char_to_byte_(char inp) {
    return static_cast<AESbyte>(inp);
  }

//Now working
  char AES::binary_to_char(AESbyte input) {
    mutex mtxx;
    unsigned int num = 0;
    string byte_str = input.to_string();
    int base = 1;
    mtxx.lock();
    for (short i = 7; i > -1; i--) {
      if (byte_str[i] == '1') {
        num += base;
      };
      base *= 2;
    };
    mtxx.unlock();
    return char(num);
  };

  string AES::binary_to_hex(AESbyte inp) {
    string output;
    stringstream ss;
    ss << hex << inp.to_ulong();
    ss >> output;
    return output;
  };

  AESbyte AES::bit_assign(int a, int b, int c, int d, int e, int f, int g, int h) {
    AESbyte result;
    result[0] = a;
    result[1] = b;
    result[2] = c;
    result[3] = d;
    result[4] = e;
    result[5] = f;
    result[6] = g;
    result[7] = h;
    return result;
  }

  AESbyte AES::hex2byte_helper(char inp, AESbyte bInp, short pos) {
    //Use pos to know if youre on the 1st or 2nd character
    //Lookup table increases performance but uses more memory
    //pos = 1 is first letter and array positions 4-7
    //pos = 2 is second letter and array position 0-3
    switch (inp) {
      case 'a':
        if (pos == 1) {
          bInp = bit_assign(bInp[0], bInp[1], bInp[2], bInp[3], 0, 1, 0, 1);
        }
        else if (pos == 2) {
          bInp = bit_assign(0, 1, 0, 1, bInp[4], bInp[5], bInp[6], bInp[7]);
        }
        break;

      case 'b':
        if (pos == 1) {
          bInp = bit_assign(bInp[0], bInp[1], bInp[2], bInp[3], 1, 1, 0, 1);
        }
        else if (pos == 2) {
          bInp = bit_assign(1, 1, 0, 1, bInp[4], bInp[5], bInp[6], bInp[7]);
        }
        break;

      case 'c':
        if (pos == 1) {
          bInp = bit_assign(bInp[0], bInp[1], bInp[2], bInp[3], 0, 0, 1, 1);
        }
        else if (pos == 2) {
          bInp = bit_assign(0, 0, 1, 1, bInp[4], bInp[5], bInp[6], bInp[7]);
        }
        break;
      
      case 'd':
        if (pos == 1) {
          bInp = bit_assign(bInp[0], bInp[1], bInp[2], bInp[3], 1, 0, 1, 1);
        }
        else if (pos == 2) {
          bInp = bit_assign(1, 0, 1, 1, bInp[4], bInp[5], bInp[6], bInp[7]);
        }
        break;

      case 'e':
        if (pos == 1) {
          bInp = bit_assign(bInp[0], bInp[1], bInp[2], bInp[3], 0, 1, 1, 1);
        }
        else if (pos == 2) {
          bInp = bit_assign(0, 1, 1, 1, bInp[4], bInp[5], bInp[6], bInp[7]);
        }
        break;

      case 'f':
        if (pos == 1) {
          bInp = bit_assign(bInp[0], bInp[1], bInp[2], bInp[3], 1, 1, 1, 1);
        }
        else if (pos == 2) {
          bInp = bit_assign(1, 1, 1, 1, bInp[4], bInp[5], bInp[6], bInp[7]);
        }
        break;
      
      case '1':
        if (pos == 1) {
          bInp = bit_assign(bInp[0], bInp[1], bInp[2], bInp[3], 1, 0, 0, 0);
        }
        else if (pos == 2) {
          bInp = bit_assign(1, 0, 0, 0, bInp[4], bInp[5], bInp[6], bInp[7]);
        }
        break;

      case '2':
        if (pos == 1) {
          bInp = bit_assign(bInp[0], bInp[1], bInp[2], bInp[3], 0, 1, 0, 0);
        }
        else if (pos == 2) {
          bInp = bit_assign(0, 1, 0, 0, bInp[4], bInp[5], bInp[6], bInp[7]);
        }
        break;

      case '3':
        if (pos == 1) {
          bInp = bit_assign(bInp[0], bInp[1], bInp[2], bInp[3], 1, 1, 0, 0);
        }
        else if (pos == 2) {
          bInp = bit_assign(1, 1, 0, 0, bInp[4], bInp[5], bInp[6], bInp[7]);
        }
        break;

      case '4':
        if (pos == 1) {
          bInp = bit_assign(bInp[0], bInp[1], bInp[2], bInp[3], 0, 0, 1, 0);
        }
        else if (pos == 2) {
          bInp = bit_assign(0, 0, 1, 0, bInp[4], bInp[5], bInp[6], bInp[7]);
        }
        break;

      case '5':
        if (pos == 1) {
          bInp = bit_assign(bInp[0], bInp[1], bInp[2], bInp[3], 1, 0, 1, 0);
        }
        else if (pos == 2) {
          bInp = bit_assign(1, 0, 1, 0, bInp[4], bInp[5], bInp[6], bInp[7]);
        }
        break;

      case '6':
        if (pos == 1) {
          bInp = bit_assign(bInp[0], bInp[1], bInp[2], bInp[3], 0, 1, 1, 0);
        }
        else if (pos == 2) {
          bInp = bit_assign(0, 1, 1, 0, bInp[4], bInp[5], bInp[6], bInp[7]);
        }
        break;
      
      case '7':
        if (pos == 1) {
          bInp = bit_assign(bInp[0], bInp[1], bInp[2], bInp[3], 1, 1, 1, 0);
        }
        else if (pos == 2) {
          bInp = bit_assign(1, 1, 1, 0, bInp[4], bInp[5], bInp[6], bInp[7]);
        }
        break;

      case '8':
        if (pos == 1) {
          bInp = bit_assign(bInp[0], bInp[1], bInp[2], bInp[3], 0, 0, 0, 1);
        }
        else if (pos == 2) {
          bInp = bit_assign(0, 0, 0, 1, bInp[4], bInp[5], bInp[6], bInp[7]);
        }
        break;

      case '9':
        if (pos == 1) {
          bInp = bit_assign(bInp[0], bInp[1], bInp[2], bInp[3], 1, 0, 0, 1);
        }
        else if (pos == 2) {
          bInp = bit_assign(1, 0, 0, 1, bInp[4], bInp[5], bInp[6], bInp[7]);
        }
        break;

      default:
        if (pos == 1) {
            bInp = bit_assign(bInp[0], bInp[1], bInp[2], bInp[3], 0, 0, 0, 0);
          }
          else if (pos == 2) {
            bInp = bit_assign(0, 0, 0, 0, bInp[4], bInp[5], bInp[6], bInp[7]);
          }
        break;
    }
    return bInp;
  }

  AESbyte AES::hex_str_to_byte(char inp1, char inp2) {
    AESbyte result;
    //individual bits can be changed by using it like an array
    //say you have byte = 0b10000000
    //the 1 is on byte[7] so it is indexed "in reverse"
    //so take the index and set it equal to either 0 or 1 to change it
    //byte[0] = 1
    //now that bit is 1 and not zero

    result = hex2byte_helper(inp1, result, 1);
    result = hex2byte_helper(inp2, result, 2);
    return result;
  }
  //////////////////////////////////////////////////////
  ////////Encrypt & Decrypt Functions///////////////////
  //////////////////////////////////////////////////////

  void AES::cypher_encrypt(AESbyte in[mtx_size], AESword w[word_size]) {  
    AESword key[4];  
    AES clone;
    for(int i=0; i<4; ++i) {
      key[i] = w[i];
    };

    clone.AddRoundKey(in, key);  
  
    for(int round=1; round<Nr; ++round) {  
      clone.SubBytes(in);  
      clone.ShiftRows(in);  
      clone.MixColumns(in);  
      for(int j=0; j<4; ++j) {
        key[j] = w[4*round+j];  
      };
      clone.AddRoundKey(in, key);  
    };  
  
    clone.SubBytes(in);  
    clone.ShiftRows(in);
    for(int k=0; k<4; ++k){
      key[k] = w[4*Nr+k];  
    };
    clone.AddRoundKey(in, key);  
  };  

  void AES::cypher_decrypt(AESbyte in[mtx_size], AESword w[word_size])  {  
    AESword key[4];
    AES clone;
    for(int i=0; i<4; ++i) {
      key[i] = w[4*Nr+i];  
    };
    clone.AddRoundKey(in, key);  

    for(int round=Nr-1; round>0; --round) {
      clone.InvShiftRows(in);  
      clone.InvSubBytes(in);  
      for(int i=0; i<4; ++i)  
          key[i] = w[4*round+i];  
      clone.AddRoundKey(in, key);  
      clone.InvMixColumns(in);  
    }  

    clone.InvShiftRows(in);  
    clone.InvSubBytes(in);  
    for(int i=0; i<4; ++i)  
        key[i] = w[i];  
    clone.AddRoundKey(in, key);  
  };  

  AESword AES::global_word[word_size];

  void AES::aes_init() {
    KeyExpansion(AES::global_word);
    AESword w[word_size];
    for (int i = 0; i < word_size; i++) {
      w[i] = global_word[i];
    };
    cypher_encrypt(SPACE, w);
    for (int i = 0; i < word_size; i++) {
      global_word[i] = w[i];
    };
  }


  ///////////////////////////////////////////////////////
  /////////Pass through encrypt & decrypt functions//////
  /////////that input and output strings/////////////////
  ///////////////////////////////////////////////////////

  /////////////
  ///Encrypt///
  /////////////

  string AES::encrypt(string input, int length) {
    string output;
    //Perhaps add something that increases the length to equal length%32 by adding ~ 's 
    AESword w[word_size];
    for (int i = 0; i < word_size; i++) {
      w[i] = global_word[i];
    };
    mutex mtxx;
    mtxx.lock();
    static unsigned int loop = 0;
    for (loop = 0; loop < length; loop+=mtx_size) {
      
      //Figure out how to convert to hex and feed AES algorithm one letter at a time
      AESbyte hex_val[mtx_size] = {SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE};// {char_to_byte_(input[loop])};
      cout << "Char 2 byte: ";
      for (int x = 0; x < mtx_size; x++) {
        if ((loop+x) < length) {
          hex_val[x] = char_to_byte_(input[loop+x]);
        }
        else {
          hex_val[x] = SPACE_BYTE;
        };
        cout << hex_val[x].to_ulong();
      };
      cout << endl;

      cypher_encrypt(hex_val, w);
      //cypher_decrypt(hex_val, w); //OH MY GOD THIS WORKS IT ACTUALLY OUTPUTTED THE STRING CORRECTLY THE PROBLEM SOULY LIES INBETWEEN THIS OUTPUTTING AND IT DECRYPTING OH MY GOD AHHHHH WHY DIDNT I THINK OF DOING THIS BEFORE

      //Make the binary turn to hex then make that a string
      cout << "EncryptedHex ";
      string output_beta;
      for (int x = 0; x < mtx_size; x++) {
        cout << hex_val[x].to_ulong();
        output_beta += binary_to_hex(hex_val[x]); //Translates correctly
      };
      cout << endl;
      cout << "Output Beta Length: " << to_string(output_beta.length()) << endl;
      if (output_beta.length() != (mtx_size*2)) {
        for (int j = 0; j != ((mtx_size*2) - output_beta.length())+1; j++) {
          //Length matches but I think I need to get rid of the % in decrypt somehow
          output_beta+="%";
          if (output_beta.length() >= (mtx_size*2)) {break;};
        };
      }
      cout << "Output beta: " << output_beta << endl;
      output += output_beta;
      
    };
    mtxx.unlock();
    for (int i = 0; i < word_size; i++) {
      global_word[i] = w[i];
    };
    return output;
  };

  /////////////
  ///Decrypt///
  /////////////

  string AES::decrypt(string input, int length) {
    string output;
    constexpr unsigned short arrSize = mtx_size;
    printf("Length: %d \n", (int)length);
    AESword w[word_size];
    for (int i = 0; i < word_size; i++) {
      w[i] = global_word[i];
    };

    static unsigned int loop = 0;

    for (loop = 0; loop < length; loop+=arrSize*2) {
      //broken, still isnt decrypting properly
      static AESbyte hex_val[arrSize] = {SPACE[0], SPACE[1], SPACE[2], SPACE[3], SPACE[4], SPACE[5], SPACE[6], SPACE[7], SPACE[8], SPACE[9], SPACE[10], SPACE[11], SPACE[12], SPACE[13], SPACE[14], SPACE[15]};
      //for loop somehow corrupted the array so im doing it manually
      printf("Loop count: %d \n", loop);
      if ((loop+1) <= length) {
        hex_val[0] = hex_str_to_byte(input[loop], input[loop+1]);
        if ((loop+3) <= length) {
          hex_val[1] = hex_str_to_byte(input[loop+2], input[loop+3]);
          if ((loop+5) <= length) {
            hex_val[2] = hex_str_to_byte(input[loop+4], input[loop+5]);
            if ((loop+7) <= length) {
              hex_val[3] = hex_str_to_byte(input[loop+6], input[loop+7]);
              if ((loop+9) <= length) {
                hex_val[4] = hex_str_to_byte(input[loop+8], input[loop+9]);
                if ((loop+11) <= length) {
                  hex_val[5] = hex_str_to_byte(input[loop+10], input[loop+11]);
                  if ((loop+13) <= length) {
                    hex_val[6] = hex_str_to_byte(input[loop+12], input[loop+13]);
                    if ((loop+15) <= length) {
                      hex_val[7] = hex_str_to_byte(input[loop+14], input[loop+15]);
                      if ((loop+17) <= length) {
                        hex_val[8] = hex_str_to_byte(input[loop+16], input[loop+17]);
                        if ((loop+19) <= length) {
                          hex_val[9] = hex_str_to_byte(input[loop+18], input[loop+19]);
                          if ((loop+21) <= length) {
                            hex_val[10] = hex_str_to_byte(input[loop+20], input[loop+21]);
                            if ((loop+23) <= length) {
                              hex_val[11] = hex_str_to_byte(input[loop+22], input[loop+23]);
                              if ((loop+25) <= length) {
                                hex_val[12] = hex_str_to_byte(input[loop+24], input[loop+25]);
                                if ((loop+27) <= length) {
                                  hex_val[13] = hex_str_to_byte(input[loop+26], input[loop+27]);
                                  if ((loop+29) <= length) {
                                    hex_val[14] = hex_str_to_byte(input[loop+28], input[loop+29]);
                                    if ((loop+31) <= length) {
                                      hex_val[15] = hex_str_to_byte(input[loop+30], input[loop+31]);
                                    };
                                  };
                                };
                              };
                            };
                          };
                        };
                      };
                    };
                  };
                };
              };
            };
          };
        };
      };                                   
      
      //Checker
      cout << "Checker: ";
      for (int y = 0; y < arrSize; y+=1) {
        cout << hex_val[y].to_ulong();
      }
      cout << endl;
      
      cypher_decrypt(hex_val, w);
      for (int n = 0; n < arrSize; n++) {
        output += binary_to_char(hex_val[n]/*.to_string()*/);
      };
    };
    for (int i = 0; i < word_size; i++) {
      global_word[i] = w[i];
    };
    printf("length %d | loop %d \n", length, loop);
    return output;
  };

  /////////////
  ///Example///
  /////////////

  int AES::start_example() {
    string actual_string = "bruh bruh Bruh Bruh Bruh Bruh a Does this actually work finally like what the frick dude this has been so long";
    
    //byte_ bit = 0b01000001; assign binary using 0b{binary} like 0b01000001

    aes_init(); //Call before use 

    //Output key
    cout << "The key is:";
    for(int i=0; i<mtx_size; ++i) {
      cout << hex << AESKEY::key[i].to_ulong() << " ";
    };
    cout << endl;

    /*AESbyte bruh[mtx_size] = {0x42, SPACE[1], SPACE[2], SPACE[3], SPACE[4], SPACE[5], SPACE[6], SPACE[7], SPACE[8], SPACE[9], SPACE[10], SPACE[11], SPACE[12], SPACE[13], SPACE[14], SPACE[15]};
    AESword w[word_size];
    for (int i = 0; i < word_size; i++) {
      w[i] = global_word[i];
    };
    AESbyte b[16] = {SPACE[0], SPACE[1], SPACE[2], SPACE[3]};
    cypher_decrypt(b, w);
    for (int i = 0; i < word_size; i++) {
      global_word[i] = w[i];
    };
    cout << "PLEASE WORK: " << bruh[0] << endl;
    */
    cout << endl << "Plaintext to be encrypted:"<< endl << actual_string << endl;

    //Encryption, output ciphertext  
    actual_string = encrypt(actual_string, actual_string.length());  
    cout << "Encrypted ciphertext:" << endl << actual_string << endl;

    //Decrypt, output plaintext 
    actual_string = decrypt(actual_string, actual_string.length());
    cout << "Decrypted plaintext:" << endl << actual_string << endl;
    cin.ignore();
    return 0;
  };
};