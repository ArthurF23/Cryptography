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

  word AES::Word(byte_& k1, byte_& k2, byte_& k3, byte_& k4) {
    word result(0x00000000);  
    word temp;  
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

  word AES::RotWord(word& rw) {
    word high = rw << 8;  
    word low = rw >> 24;  
    return high | low;  
  };

  word AES::SubWord(word& sw)  {  
    word temp;  
    for(int i=0; i<32; i+=8)  {  
        int row = sw[i+7]*8 + sw[i+6]*4 + sw[i+5]*2 + sw[i+4];  
        int col = sw[i+3]*8 + sw[i+2]*4 + sw[i+1]*2 + sw[i];  
        byte_ val = AES::S_Box[row][col];  
        for(int j=0; j<8; ++j)  
            temp[i+j] = val[j];  
    };  
    return temp;  
  };

  void AES::KeyExpansion(word w[4*(Nr+1)]) {  
    word temp;  
    int i = 0;  
    //The first four of w [] are input key s  
    while(i < Nk) {  
        w[i] = Word(AESKEY::key[4*i], AESKEY::key[4*i+1], AESKEY::key[4*i+2], AESKEY::key[4*i+3]);  
        ++i;  
    };
  
    i = Nk;  
  
    while(i < 4*(Nr+1)) {  
      temp = w[i-1]; //Record the previous word  
      if(i % Nk == 0) {
        word rwt = RotWord(temp);
        w[i] = w[i-Nk] ^ SubWord(rwt) ^ Rcon[i/Nk-1];  
      }
      else {
        w[i] = w[i-Nk] ^ temp;  
      };
      ++i;  
    };  
  };  

  void AES::SubBytes(byte_ mtx[mtx_size]) {  
    for(int i=0; i<mtx_size; ++i)  {  
        int row = mtx[i][7]*8 + mtx[i][6]*4 + mtx[i][5]*2 + mtx[i][4];  
        int col = mtx[i][3]*8 + mtx[i][2]*4 + mtx[i][1]*2 + mtx[i][0];  
        mtx[i] = S_Box[row][col];  
    };
  };

  void AES::ShiftRows(byte_ mtx[mtx_size]) {  
    //The second line circle moves one bit to the left  
    byte_ temp = mtx[4];  
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
    
  byte_ AES::GFMul(byte_ a, byte_ b) {   
      byte_ p = 0;  
      byte_ hi_bit_set;  
      for (int counter = 0; counter < 8; counter++) {  
          if ((b & byte_(1)) != 0) {  
              p ^= a;  
          }  
          hi_bit_set = (byte_) (a & byte_(0x80));  
          a <<= 1;  
          if (hi_bit_set != 0) {  
              a ^= 0x1b; /* x^8 + x^4 + x^3 + x + 1 */  
          }  
          b >>= 1;  
      }  
      return p;  
  };

  void AES::MixColumns(byte_ mtx[mtx_size])  {  
    byte_ arr[4];  
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

  void AES::AddRoundKey(byte_ mtx[mtx_size], word k[4]) {  
    for(int i=0; i<4; ++i) {  
      word k1 = k[i] >> 24;  
      word k2 = (k[i] << 8) >> 24;  
      word k3 = (k[i] << 16) >> 24;  
      word k4 = (k[i] << 24) >> 24;  
        
      mtx[i] = mtx[i] ^ byte_(k1.to_ulong());  
      mtx[i+4] = mtx[i+4] ^ byte_(k2.to_ulong());  
      mtx[i+8] = mtx[i+8] ^ byte_(k3.to_ulong());  
      mtx[i+12] = mtx[i+12] ^ byte_(k4.to_ulong());  
    };  
  };

  void AES::InvSubBytes(byte_ mtx[mtx_size]) {  
    for(int i=0; i<mtx_size; ++i) {  
      int row = mtx[i][7]*8 + mtx[i][6]*4 + mtx[i][5]*2 + mtx[i][4];  
      int col = mtx[i][3]*8 + mtx[i][2]*4 + mtx[i][1]*2 + mtx[i][0];  
      mtx[i] = Inv_S_Box[row][col];
    };
  };

  void AES::InvShiftRows(byte_ mtx[mtx_size]) {  
    //The second line circle moves one bit to the right  
    byte_ temp = mtx[7];  
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

  void AES::InvMixColumns(byte_ mtx[mtx_size]) {  
    byte_ arr[4];  
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

  string AES::hex_to_string(byte_ inp[mtx_size]) {
    string output;
    for (int i=0; i < mtx_size; i++) {
      output += static_cast<char>(inp[i].to_ulong());
    };
    return output;
  }

  void AES::generate_key() {
    
  };

  //Basically converting char to hex
  byte_ AES::char_to_byte_(char inp) {
    byte_ result;
    char *l_pCharRes = new (char);
    sprintf(l_pCharRes, "%X", int(inp));
    int l_intResult = stoi(l_pCharRes);
    result = l_intResult;
    return result;
  }

  //////////////////////////////////////////////////////
  ////////Encrypt & Decrypt Functions///////////////////
  //////////////////////////////////////////////////////

  void AES::cypher_encrypt(byte_ in[mtx_size], word w[4*(Nr+1)]) {  
    word key[4];  
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

  void AES::cypher_decrypt(byte_ in[mtx_size], word w[4*(Nr+1)])  {  
    word key[4];
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


  ///////////////////////////////////////////////////////
  /////////Pass through encrypt & decrypt functions//////
  /////////that input and output strings/////////////////
  ///////////////////////////////////////////////////////

  string AES::encrypt(string input) {
    string output;
    word w[4*(Nr+1)];
    const int inp_len = input.length();
    printf("%d \n", inp_len);
    mutex mtxx;
    mtxx.lock();
    static unsigned int loop;
    for (loop = 0; loop < 10; loop++) {
      //Figure out how to convert to hex and feed AES algorithm one letter at a time
      byte_ hex_val[1] = {char_to_byte_(input[loop])};
      //cout << "d:" << hex_val[0] << endl;
      cypher_encrypt(hex_val, w);
      //cout << "e:" <<  hex_val[0] << endl;
      printf("%d \n", loop);
      
      //make it not be binary but an actual char value
      output += hex_val[0].to_string();
    };
    mtxx.unlock();
    cout << "Output: " << output << endl;
    return output;
  };

  string AES::decrypt(string input) {
    string output;

    return output;
  };

  void AES::start_example() {
    byte_ plain[mtx_size] = {0x54, 0x65, 0x73, 0x74, 0x20, 0x45, 0x78, 0x61,  0x6D, 0x70, 0x6C, 0x65, 0x2E, 0x2E, 0x2E, 0x2E};

    string actual_string = "bruh lmfao";

      //Output key  
    cout << "The key is:";  
    for(int i=0; i<mtx_size; ++i) {
      cout << hex << AESKEY::key[i].to_ulong() << " ";
    };
    cout << endl;

    word w[4*(Nr+1)];  
    KeyExpansion(w);

    cout << endl << "Plaintext to be encrypted:"<< endl << actual_string << endl;

     //Encryption, output ciphertext  
    actual_string = encrypt(actual_string);  
    cout << "Encrypted ciphertext:"<< endl << actual_string << endl; 

    //Decrypt, output plaintext  
    cypher_decrypt(plain, w);  
    cout << "Decrypted plaintext:"<< endl << hex_to_string(plain) << endl;
  };

};