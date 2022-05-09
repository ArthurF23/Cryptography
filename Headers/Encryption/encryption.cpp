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
          ">.__",
          ">_..",
          ">.>_",
          ">>>>",
          "____",
          "....",
          ">_>>",
          "_.>>"
        };
        
  bool encdec::isNumberString(string input) {
    for (char &c : input) {
      if (!isdigit(c)) {return false;};
    };
    return true;
  }

  unsigned int encdec::get_random_num(unsigned int min, unsigned int max) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(min,max);
    return dist6(rng);
  }

  unsigned int encdec::decide_scramble() {
    for (int i=encdec::constants::scramble_info::starting_divisor; i < encdec::constants::scramble_info::ending_divisor+1; i++) {
      if ((KEY::key%i) == 0) {
        return i;
      };
    };
    return 1;
  }

  unsigned int encdec::generate_key() {
    return encdec::get_random_num(encdec::constants::key_info::key_min_value, encdec::constants::key_info::key_max_value);
  }

  char encdec::generate_ASCII_char() {
    return (char)encdec::get_random_num(encdec::constants::char_info::valid_char_min, encdec::constants::char_info::valid_char_max);
  }

  bool encdec::assign_key(string inp) {
    if (encdec::isNumberString(inp)) {
      if (inp.length() == encdec::constants::key_info::key_length && stoi(inp) >= encdec::constants::key_info::key_min_value) {
        KEY::key = stoi(inp);
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
  };
  
  /////////////////////////////////////////////////////////
  ///////// AES ///////////////////////////////////////////
  /////////////////////////////////////////////////////////

  AESword AES::_4Bytes2Word(AESbyte& k1, AESbyte& k2, AESbyte& k3, AESbyte& k4) {
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

  AESword AES::PosTrans(AESword& rw) {
    AESword high = rw << 8;  
    AESword low = rw >> 24;  
    return high | low;
  };

  AESword AES::SBoxTrans(AESword& sw)  {  
    AESword temp;  
    for(int i=0; i<32; i+=bitsInByte)  {  
        int row = sw[i+7]*bitsInByte + sw[i+6]*4 + sw[i+5]*2 + sw[i+4];  
        int col = sw[i+3]*bitsInByte + sw[i+2]*4 + sw[i+1]*2 + sw[i];  
        AESbyte val = AES::S_Box[row][col];  
        for(int j=0; j<bitsInByte; ++j)  
            temp[i+j] = val[j];  
    };  
    return temp;  
  };

  void AES::KeyExpansion(AESword w[expanded_key_size]) {  
    AESword temp;
    int i = 0;  
    //The first four of w [] are input keys  
    while(i < AESwords_in_key) {  
        w[i] = _4Bytes2Word(AES::KEY::key[4*i], AES::KEY::key[4*i+1], AES::KEY::key[4*i+2], AES::KEY::key[4*i+3]);  
        ++i;  
    };
  
    i = AESwords_in_key;  
  
    while(i < expanded_key_size) {  
      temp = w[i-1]; //Record the previous word  
      if(i % AESwords_in_key == 0) {
        AESword rwt = PosTrans(temp);
        w[i] = w[i-AESwords_in_key] ^ SBoxTrans(rwt) ^ WheelConst[i/AESwords_in_key-1];  
      }
      else {
        w[i] = w[i-AESwords_in_key] ^ temp;  
      };
      ++i;  
    };
  };

  void AES::SBoxTransSubBytes(AESbyte mtx[mtx_size]) {  
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
  AESbyte AES::GaloisFieldsMul(AESbyte a, AESbyte b) {   
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
        mtx[i] = GaloisFieldsMul(0x02, arr[0]) ^ GaloisFieldsMul(0x03, arr[1]) ^ arr[2] ^ arr[3];  
        mtx[i+4] = arr[0] ^ GaloisFieldsMul(0x02, arr[1]) ^ GaloisFieldsMul(0x03, arr[2]) ^ arr[3];  
        mtx[i+8] = arr[0] ^ arr[1] ^ GaloisFieldsMul(0x02, arr[2]) ^ GaloisFieldsMul(0x03, arr[3]);  
        mtx[i+12] = GaloisFieldsMul(0x03, arr[0]) ^ arr[1] ^ arr[2] ^ GaloisFieldsMul(0x02, arr[3]);  
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

  void AES::InvSBoxTransSubBytes(AESbyte mtx[mtx_size]) {  
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
        mtx[i] = GaloisFieldsMul(0x0e, arr[0]) ^ GaloisFieldsMul(0x0b, arr[1]) ^ GaloisFieldsMul(0x0d, arr[2]) ^ GaloisFieldsMul(0x09, arr[3]);  
        mtx[i+4] = GaloisFieldsMul(0x09, arr[0]) ^ GaloisFieldsMul(0x0e, arr[1]) ^ GaloisFieldsMul(0x0b, arr[2]) ^ GaloisFieldsMul(0x0d, arr[3]);  
        mtx[i+8] = GaloisFieldsMul(0x0d, arr[0]) ^ GaloisFieldsMul(0x09, arr[1]) ^ GaloisFieldsMul(0x0e, arr[2]) ^ GaloisFieldsMul(0x0b, arr[3]);  
        mtx[i+12] = GaloisFieldsMul(0x0b, arr[0]) ^ GaloisFieldsMul(0x0d, arr[1]) ^ GaloisFieldsMul(0x09, arr[2]) ^ GaloisFieldsMul(0x0e, arr[3]);  
    };
  };

  //Basically converting char to hex
  AESbyte AES::CONVERSIONS::char_to_byte_(char inp) {
    return static_cast<AESbyte>(inp);
  };

  char AES::CONVERSIONS::binary_to_char(AESbyte input) {
    unsigned int num = 0;
    string byte_str = input.to_string();
    int base = 1;
    for (short i = 7; i > -1; i--) {
      if (byte_str[i] == '1') {
        num += base;
      };
      base *= 2;
    };
    return char(num);
  };

  unsigned int AES::getRandomNum(unsigned int min, unsigned int max) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(min,max);
    return dist6(rng);
  };

  void AES::generate_key() {
    for (int x = 0; x < mtx_size; x++) {
      AESbyte bin = 0b00000000;
      for (int i = 0; i < bitsInByte; i++) {
        bin[i] = getRandomNum(0, 1);
      };
      AES::KEY::key[x] = bin;
    };
  };

  AESword AES::global_expanded_key[expanded_key_size];

  void AES::aes_init(OPTIONS genkey, string dummykey) {
    if (genkey == OPTIONS::doGenerateKey || dummykey.length() < mtx_size*bitsInByte) {
      generate_key();
      }
    else {
      for (int y = 0, n=0; y < mtx_size; y++, n+=bitsInByte) {
        AES::KEY::key[y] = CONVERSIONS::binStr_to_byte(dummykey.substr(n, n+bitsInByte));
      };
    };
    KeyExpansion(AES::global_expanded_key);
  };

  AESbyte AES::CONVERSIONS::binStr_to_byte(string input) {
    AESbyte result;
    for (int i = 0, y = bitsInByte-1; i < bitsInByte; i++, y--) {
      if (input[y] == '1') {
        result[i] = 1;
      }
    };
    return result;
  }
  
  //////////////////////////////////////////////////////
  ////////Encrypt & Decrypt Functions///////////////////
  //////////////////////////////////////////////////////

  void AES::cypher_encrypt(AESbyte in[mtx_size], AESword w[expanded_key_size]) {
    AESword key[4];  
    AES clone;
    for(int i=0; i<4; ++i) {
      key[i] = w[i];
    };
    clone.AddRoundKey(in, key);  
    for(int round=1; round<rounds_of_encryption; ++round) {  
      clone.SBoxTransSubBytes(in);  
      clone.ShiftRows(in);  
      clone.MixColumns(in);  
      for(int j=0; j<4; ++j) {
        key[j] = w[4*round+j];  
      };
      clone.AddRoundKey(in, key);  
    };
    clone.SBoxTransSubBytes(in);  
    clone.ShiftRows(in);
    for(int k=0; k<4; ++k){
      key[k] = w[4*rounds_of_encryption+k];  
    };
    clone.AddRoundKey(in, key);
  };  

  void AES::cypher_decrypt(AESbyte in[mtx_size], AESword w[expanded_key_size])  {
    AESword key[4];
    AES clone;
    for(int i=0; i<4; ++i) {
      key[i] = w[4*rounds_of_encryption+i];  
    };
    clone.AddRoundKey(in, key);  
    for(int round=rounds_of_encryption-1; round>0; --round) {
      clone.InvShiftRows(in);  
      clone.InvSBoxTransSubBytes(in);  
      for(int i=0; i<4; ++i)  
          key[i] = w[4*round+i];
      clone.AddRoundKey(in, key);  
      clone.InvMixColumns(in);  
    }  
    clone.InvShiftRows(in);  
    clone.InvSBoxTransSubBytes(in);  
    for(int i=0; i<4; ++i)  
        key[i] = w[i];  
    clone.AddRoundKey(in, key);  
  };  


  ///////////////////////////////////////////////////////
  /////////Pass through encrypt & decrypt functions//////
  /////////that input and output strings/////////////////
  ///////////////////////////////////////////////////////
  AESbyte AES::KEY::key[mtx_size]; //Key definition

  /////////////
  ///Encrypt///
  /////////////

  string AES::encrypt(string input) {
    unsigned int length = input.length();
    string output;
    AESword w[expanded_key_size];
    for (int i = 0; i < expanded_key_size; i++) {
      w[i] = global_expanded_key[i];
    };
    for (unsigned int loop = 0; loop < length; loop+=mtx_size) {
      
      AESbyte hex_val[mtx_size] = {SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE};

      //convert char to binary byte
      for (int x = 0; x < mtx_size; x++) {
        if ((loop+x) < length) {
          hex_val[x] = CONVERSIONS::char_to_byte_(input[loop+x]);
        }
        else {
          hex_val[x] = SPACE_BYTE;
        };
      };

      cypher_encrypt(hex_val, w);
      //Make the binary turn to hex then make that a string
      for (int x = 0; x < mtx_size; x++) {
        output += hex_val[x].to_string(); //Binary output
      };
      
    };
    for (int i = 0; i < expanded_key_size; i++) {
      global_expanded_key[i] = w[i];
    };
    binary_compression::compress(output);
    return output;
  };

  /////////////
  ///Decrypt///
  /////////////

  string AES::decrypt(string input) {
    binary_compression::decompress(input);
    unsigned int length = input.length();
    string output;
    constexpr unsigned short arrSize = mtx_size;
    AESword w[expanded_key_size];
    for (int i = 0; i < expanded_key_size; i++) {
      w[i] = global_expanded_key[i];
    };

    for (unsigned int loop = 0; loop < length; loop+=arrSize*bitsInByte) {
      static AESbyte hex_val[arrSize];
      for (int x = loop, p = 0; p < arrSize; x+=bitsInByte, p++) {
        hex_val[p] = CONVERSIONS::binStr_to_byte(input.substr(x, x+bitsInByte));
      };
      
      cypher_decrypt(hex_val, w);
      for (int n = 0; n < arrSize; n++) {
        if (hex_val[n] != SPACE_BYTE) {
          output += CONVERSIONS::binary_to_char(hex_val[n]);
        }
      };
    };
    for (int i = 0; i < expanded_key_size; i++) {
      global_expanded_key[i] = w[i];
    };
    return output;
  };

  ////////////////
  ///File Stuff///
  ////////////////

  bool AES::encryptFF(string path) {
    string output;
    //Check if path is good
    ifstream infile(path);
    if (infile.good() == false) {return false;};
    //Get text from file
    string text;
    string line;
    while (getline(infile, line)) {
      text += line + '\n';
    };
    infile.close();
    text = encrypt(text);
    ofstream outfile(path, ios::out | ios::trunc);
    outfile << text;
    outfile.close();
    return true;
  };

  bool AES::decryptFF(string path) {
    string output;
    //Check if path is good
    ifstream infile(path);
    if (infile.good() == false) {return false;};
    //Get text from file
    string text;
    getline(infile, text); 
    infile.close();
    text = decrypt(text);
    ofstream outfile(path, ios::out | ios::trunc);
    outfile << text;
    outfile.close();
    return true;
  };

  ///////////////////////////////////
  ///Making my own encrypted files///
  ///////////////////////////////////

  void AES::FILES::TXT::get(string path, string& data) {
    ifstream infile(path, ios::binary);
    //get length of file:
    infile.seekg (0, infile.end);
    int length = infile.tellg();
    infile.seekg (0, infile.beg);

    //buffer for data
    char * buffer = new char [length];

    // read data as a block:
    infile.read(buffer,length);
    data += buffer;
    //Don't need this anymore sooo...
    delete[] buffer;
    infile.close();    
  };

  void AES::FILES::TXT::out(string path, string data) {
    //Original File
    ofstream {path}; //Create... Doesn't matter if it's overwritten because it's about to be anyways
    ofstream outfile(path, ios::out | ios::trunc);
    outfile << data; //write to file
    outfile.close(); //close file
  }

  void AES::FILES::BMP_::get(string path, string& data) {
    char* filename = const_cast<char*>(path.c_str());
    BMPbyte *pixels;
    int32 width, height, bytesPerPixel;
    //Get size of pixel arr with width*height*bytesPerPixel
    BMP::ReadImage(filename, &pixels, &width, &height, &bytesPerPixel);
    data+=to_string(width)+FILES::BMP_::DATA_SEPARATOR;
    data+=to_string(height)+FILES::BMP_::DATA_SEPARATOR;
    data+=to_string(bytesPerPixel)+FILES::BMP_::DATA_SEPARATOR;
    string pix;
    for (unsigned int i=0; i<(width*height*bytesPerPixel); i++) {
      pix+=to_string(pixels[i]) + NUM_SEPARATOR;
    };
    
    rgb_compression::compress(pix, NUM_SEPARATOR);
    
    data+=pix;
    pix.clear();
  };

  void AES::FILES::BMP_::out(string path, string data) {
    //Convert path str to char*
    char* filename = const_cast<char*>(path.c_str());
    data.erase(0, data.find_first_of(AES::FILES::EXTENSION_SEPERATOR)+1);
    int32 width, height, bytesPerPixel;
    string str;
    //get width
    str = data.substr(0, data.find_first_of(DATA_SEPARATOR));
    width = stoi(str);
    //get height
    data.erase(0, data.find_first_of(DATA_SEPARATOR)+1);
    str = data.substr(0, data.find_first_of(DATA_SEPARATOR));
    height = stoi(str);
    //get bytesPerPixel
    data.erase(0, data.find_first_of(DATA_SEPARATOR)+1);
    str = data.substr(0, data.find_first_of(DATA_SEPARATOR));
    bytesPerPixel = stoi(str);
    data.erase(0, data.find_first_of(DATA_SEPARATOR)+1);

    rgb_compression::decompress(data, NUM_SEPARATOR);
    //Length of pixels
    const unsigned int length = height*(width*bytesPerPixel);
    
    BMPbyte* pixels;    
    rgb_compression::asgnPix(pixels, data, NUM_SEPARATOR, length); data.clear();
    
    //Write the image
    BMP::WriteImage(filename, pixels, width, height, bytesPerPixel);
  };

  //Design: file type will be in front of all the data.
  //It will be seperated with a ~ in the decrypted code.

  //so it will be like this
  //.{extension}~{data}
  //In the case of bitmaps it will be like this
  //.bmp~{width}:{height}:{bytesPerPixel}:{rgb int},{rgb int},etc...
  
  //Extension: .aesenc
  const string AES::FILES::FILE_EXTENSION = ".aesenc";
  const string AES::FILES::KEYFILE_NAME = "_KEYFILE";
  const string AES::FILES::KEYFILE_EXT = ".aeskey";
  const string AES::FILES::TXT::identifier[id_len] = {".txt", ".md", ".cpp", ".h", ".cs", ".c"};
  const string AES::FILES::BMP_::identifier = ".bmp";
  //Classify type of file
  void AES::FILES::classify(string ext, AES::FILES::CLASSIFIER &type) {
    for (int i=0; i < AES::FILES::TXT::id_len; i++) {
      if (ext == FILES::TXT::identifier[i]) {type=FILES::CLASSIFIER::_TEXT;break;};
    };
    
    if (ext == FILES::BMP_::identifier) {type=FILES::CLASSIFIER::_BITMAP;};
  };

  //Key File
  bool AES::FILES::gen_key_file(string path) {
    path.erase(path.rfind('.'), path.length()); //Erase extension
    path+=KEYFILE_NAME;//edit name
    path+=KEYFILE_EXT;//add extension
    ofstream outFile(path);
    if (outFile.good() == false) {outFile.close(); return false;};
    
    string data;
    for (int i = 0; i < AES::mtx_size; i++) {
      data+=AES::KEY::key[i].to_string();
    };

    size_t key;
    data = AKARE::encrypt(data, key);
    
    binary_compression::compress(data);

    data = to_string(key) + '\n' + data;
    
    outFile << data;
    outFile.close();
    return true;
  };

  bool AES::FILES::in_key_file(string path) {
    path.erase(path.rfind('.'), path.length()); //Erase extension
    path+=KEYFILE_NAME;//edit name
    path+=KEYFILE_EXT;//add extension
    ifstream inFile(path);
    if (inFile.good() == false) {inFile.close(); return false;};
    inFile.close();
    string data;
    AES::FILES::TXT::get(path, data);

    short fst = data.find_first_of('\n');

    if (fst != string::npos) {
      string ky = data.substr(0, fst);

      data = data.substr(fst+1, data.length()-fst);
      
      binary_compression::decompress(data);
  
      stringstream ss;
      ss << ky;
      size_t key;
      ss >> key;
      
      data = AKARE::decrypt(data, key);
    } else {
      binary_compression::decompress(data);
    };
    
    
    aes_init(AES::OPTIONS::noGenerateKey, data);
    return true;
  };

  bool AES::encryptFile(string path, FILE_FLAGS flags) {
    //checks if path is valid
    if(FileOP::isRealDir(path) == false) {return false;};
    
    AES::FILES::gen_key_file(path);

    string ext = path.substr(path.find_last_of('.'), path.length());

    //Add the extension + seperator to the data string
    string data = ext + FILES::EXTENSION_SEPERATOR;

    FILES::CLASSIFIER type = FILES::CLASSIFIER::_RETURN;
    FILES::classify(ext, type);
    
    switch (type) {
      case FILES::CLASSIFIER::_RETURN:
        return false;
      case FILES::CLASSIFIER::_TEXT:
        FILES::TXT::get(path, data);
        break;
      case FILES::CLASSIFIER::_BITMAP:
        FILES::BMP_::get(path,data);
        break;      
      default:
        return false;
    };
    
    ext.clear(); //Delete because it's useless
    
    //Encrypt it
    data = encrypt(data);
      
    //Make new file & path using old path by removing the extension from the string
    if (flags == FILE_FLAGS::deleteInputFile) {remove(path.c_str());};
    path.erase(path.rfind('.'), path.length());
    path+=FILES::FILE_EXTENSION;
    //Simple write so why not use txt out
    FILES::TXT::out(path, data);
    return true;
  };

  bool AES::decryptFile(string path, string keyFilePath, FILE_FLAGS flags) {
     //checks if path is valid
     if(FileOP::isRealDir(path) == false || path.substr(path.find_last_of('.'), path.length()) != FILES::FILE_EXTENSION) {return false;}
    string data;
    //Just like in encryptFile
    AES::FILES::TXT::get(path, data);

    if (keyFilePath != "") {AES::FILES::in_key_file(keyFilePath);}
    else {AES::FILES::in_key_file(path);};    
    
    //Decrypt
    data = decrypt(data);

    if (flags & AES::FILE_FLAGS::deleteAesencFile) {remove(path.c_str());};
    if (flags & AES::FILE_FLAGS::deleteKeyFile) {
      string pa = path;
      pa.erase(pa.rfind('.'), pa.length()); //Erase extension
      pa+=FILES::KEYFILE_NAME;//edit name
      pa+=FILES::KEYFILE_EXT;//add extension
      remove(pa.c_str()); pa.clear();
    };
    
    //Make new path
    path.erase(path.find_last_of('.'), path.length());
    string ext = data.substr(data.find_first_of('.'), data.find_first_of(FILES::EXTENSION_SEPERATOR));
    path += ext; //add extension to path

    //Erase that part of the decrypted data
    data.erase(data.find_first_of('.'), data.find_first_of(FILES::EXTENSION_SEPERATOR)+1);
    data.erase(data.length(), data.length());

    FILES::CLASSIFIER type = FILES::CLASSIFIER::_RETURN;
    FILES::classify(ext, type);
    ext.clear(); //Not needed, free memory
    
    switch (type) {
      case FILES::CLASSIFIER::_RETURN:
        return false;
      
      case FILES::CLASSIFIER::_TEXT:
        FILES::TXT::out(path,data);
        break;
      
      case FILES::CLASSIFIER::_BITMAP:
        FILES::BMP_::out(path,data);
        break;
      
      default:
        return false;
    };
    return true;
  };

  /////////////////////////////////
  ///DUO encryption & decryption///
  /////////////////////////////////
  
  //last 2 params are overloaded
  string DUO::encrypt(string input, encryption::encdec::FLAGS bloat, encryption::encdec::FLAGS pattern) {
    input = encryption::AES::encrypt(input);
    input = encryption::encdec::encrypt(input, bloat, pattern);
    return input;
  };

  string DUO::decrypt(string input) {
    input = encryption::encdec::decrypt(input);
    input = encryption::AES::decrypt(input);
    return input;
  };


  //Initilize both functions to be ready.
  //1st param this is the key for encdec, you need to input this
  //2nd param this is the option to generate a key, select false then fill the third param. this defaults to generate key so if you dont, then input the proper OPTION to select false then fill out the 3rd param
  //3rd param this is the string of 16 bytes that is the AES key youll need to input this if you dont want to generate a key
  void DUO::init(string encdecKey, encryption::AES::OPTIONS genKey, string aesKey) {
    //encdec init key
    if (!encryption::encdec::assign_key(encdecKey)) {
      return;
    };

    //AES init
    encryption::AES::aes_init(genKey, aesKey);
  };

  /////////////////////
  ///Vigenere Cypher///
  /////////////////////

  int vigenere::translate(char message) {
    switch (message) {
    case 'A':
      return 0;
    case 'B':
      return 1;
    case 'C':
      return 2;
    case 'D':
      return 3;
    case 'E':
      return 4;
    case 'F':
      return 5;
    case 'G':
      return 6;
    case 'H':
      return 7;
    case 'I':
      return 8;
    case 'J':
      return 9;
    case 'K':
      return 10;
    case 'L':
      return 11;
    case 'M':
      return 12;
    case 'N':
      return 13;
    case 'O':
      return 14;
    case 'P':
      return 15;
    case 'Q':
      return 16;
    case 'R':
      return 17;
    case 'S':
      return 18;
    case 'T':
      return 19;
    case 'U':
      return 20;
    case 'V':
      return 21;
    case 'W':
      return 22;
    case 'X':
      return 23;
    case 'Y':
      return 24;
    case 'Z':
      return 25;


    case 'a':
      return 0;
    case 'b':
      return 1;
    case 'c':
      return 2;
    case 'd':
      return 3;
    case 'e':
      return 4;
    case 'f':
      return 5;
    case 'g':
      return 6;
    case 'h':
      return 7;
    case 'i':
      return 8;
    case 'j':
      return 9;
    case 'k':
      return 10;
    case 'l':
      return 11;
    case 'm':
      return 12;
    case 'n':
      return 13;
    case 'o':
      return 14;
    case 'p':
      return 15;
    case 'q':
      return 16;
    case 'r':
      return 17;
    case 's':
      return 18;
    case 't':
      return 19;
    case 'u':
      return 20;
    case 'v':
      return 21;
    case 'w':
      return 22;
    case 'x':
      return 23;
    case 'y':
      return 24;
    case 'z':
      return 25;
    default:
      return -1; //exclude char
    }
    return -1;
  };

  int vigenere::translate(char message, char key) {
    int key_row;
    int letter_num;
    //Find Key Row
    for (int i = 0; i < 26; i++) {
      if (key == vigenere::table[i][0]) {
        key_row = i;
        break;
      }
    }
    //Find Letter In Key Row
    for (int i = 0; i < 26; i++) {
      if (vigenere::table[key_row][i] == message) {
        letter_num = i;
      }
    }

    switch (letter_num) {
    case 0:
      return table[0][0];
    case 1:
      return table[0][1];
    case 2:
      return table[0][2];
    case 3:
      return table[0][3];
    case 4:
      return table[0][4];
    case 5:
      return table[0][5];
    case 6:
      return table[0][6];
    case 7:
      return table[0][7];
    case 8:
      return table[0][8];
    case 9:
      return table[0][9];
    case 10:
      return table[0][10];
    case 11:
      return table[0][11];
    case 12:
      return table[0][12];
    case 13:
      return table[0][13];
    case 14:
      return table[0][14];
    case 15:
      return table[0][15];
    case 16:
      return table[0][16];
    case 17:
      return table[0][17];
    case 18:
      return table[0][18];
    case 19:
      return table[0][19];
    case 20:
      return table[0][20];
    case 21:
      return table[0][21];
    case 22:
      return table[0][22];
    case 23:
      return table[0][23];
    case 24:
      return table[0][24];
    case 25:
      return table[0][25];
    default:
      break;
    };
    return -1;
  };

  string vigenere::ModKey(string inp) {
    string output;
    //Remove spaces
    for (int i=0; i<inp.length(); i++) {
      if (inp[i] != ' ') {
        output += inp[i];
      };
    };
    //Make caps
    return output;
  };

  string vigenere::generate_key(int message_length) {
    string output;
		for (int i = 0; i < message_length; i++) {
			std::random_device rd;
			std::mt19937 rng(rd());
			std::uniform_int_distribution<int> uni(minGenVal, maxGenVal);
			char letter = uni(rng);
			output += letter;
		}
    return output;
  };

  void vigenere::init(int messageLength) {
    VIGENERE_KEY::key = generate_key(messageLength);
  };

  void vigenere::init(string inpKey) {
    VIGENERE_KEY::key = ModKey(inpKey);
  };

  /////////////////////////////
  ///Encryption & Decryption///
  /////////////////////////////
  string VIGENERE_KEY::key = "NULL";
  string vigenere::encrypt(string input) {
    init(input.length());
    string output;
    for (int i = 0; i < input.length(); i++) {
			int x = translate(input[i]); // message
			int y = translate(VIGENERE_KEY::key[i]); // key
			if (x != -1 && y != -1) {
        input[i] = table[y][x];
			};
		};
    
    return input;
  };

  string vigenere::decrypt(string input, string _key) {
    init(_key);
		for (int i = 0; i < input.length(); i++) {
			input[i] = translate(input[i], VIGENERE_KEY::key[i]);
		};
    return input;
  };
};