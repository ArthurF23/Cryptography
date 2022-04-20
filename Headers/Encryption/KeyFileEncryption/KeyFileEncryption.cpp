#include "KeyFileEncryption.h"
namespace asymmetric {

  //Ensures numbers are valid
  bool AKARE::isValidNum(size_t num) {
    float* div = (float*)malloc(sizeof(float));
    *div = (float)(num) / (float)(staticPrivateKey);
    if (num%2 == 0 && isDecimal(*div) == false) {free(div); return true;}
    else {free(div); return false;};
  };

  //Backend for generation of random even numbers
  size_t AKARE::evenGeneratorBackend(size_t min, size_t max, size_t loop) {
    if (loop >= 200) {return 0;}; loop++;
    
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<mt19937::result_type> dist6(min,max);
    
    size_t* val = (size_t*)malloc(sizeof(size_t));
    *val = dist6(rng);
    if (isValidNum(*val)) {return *val;}
    else {
      free(val);
      return evenGeneratorBackend(min, max, loop); 
    };
  };

  //Generate an even number
  size_t AKARE::evenGenerator(size_t min, size_t max) {
    size_t gen = evenGeneratorBackend(min, max);
    if (gen == 0) {return evenGenerator();}
    else {return gen;};
  };

  //Check if num is decimal
  bool AKARE::isDecimal(float inp) {
    if (size_t(inp) != inp) {return true;}
    else {return false;};
  };

  //Generate valid even number based on the public key
  size_t AKARE::mod(size_t inp) {
    seeder(inp);
    srand(inp);
    size_t val = rand();
    while (1) {
      if (!isValidNum(val)) {val = rand();}
      else {break;};
    };
    return val;
  };

  //Makes mod() seed for srand more random with a custom algorithm
  void AKARE::seeder(size_t &inp) {
    if (inp > bounds::genMin*2) {inp/=2;};
    
    stringstream ss;
    string str;
    ss << inp;
    ss >> str;
    
    if (str[0]-'0' > 5) {inp*=4;}; 
    
    ss.str("");
    ss << inp;
    ss >> str;
    ss.str("");
    
    if (str[2] < 5) {inp*=6;} else {inp*=8;};
    
    srand(inp);
    unsigned short ra = rand() % 100 + 1;
    
    if (ra > 50) {
      inp*=4;
      srand(inp+ra);
      if ((rand() % 100 + 1) > 50) {inp*=4;} else {inp/=2;};
    } 
    else {
      inp/=2;
      srand(inp*ra);
      if ((rand() % 100 + 1) > 50) {inp*=8;} else {inp/=2;};
    };

    srand(inp);
    ra = rand() % 100 + 1;
    if (ra < 20) {inp*=ra;} else {inp*=2;};
    
    unsigned short ra2 = rand() % 100 + 1, ra3 = rand() % 100 + 1;

    if (ra < ra2 && ra2 > ra3) {inp/=2;}
    else if (ra2 < ra && ra3 > ra) {inp*=2;}
    else if (ra < 70 && ra2 < 50 && ra3 < 60) {inp*=6;}
    else {inp*=8;};
    
    srand (ra+ra2+ra3+inp);
    if ((rand() % 100 + 1) >= 50) {inp/=2;} else {inp*=4;};
  };

  string AKARE::shuffle8(string inp, size_t looper) {
    
    if (inp.length() != 8) {return "ERR INVALID LENGTH";};
    for (size_t i = 0; i < looper; i++) {
      string str;
      str = inp[7];
      str += inp[2];
      str += inp[3];
      str += inp[1];
      str += inp[0];
      str += inp[6];
      str += inp[4];
      str += inp[5];
      inp = str;
    };       
    return inp;
  };

  string AKARE::unshuffle8(string inp, size_t looper) {
    if (inp.length() != 8) {return "ERR INVALID LENGTH";};
    for (size_t i = 0; i < looper; i++) {
      string str;
      str = inp[4];
      str += inp[3];
      str += inp[1];
      str += inp[2];
      str += inp[6];
      str += inp[7];
      str += inp[5];
      str += inp[0];
      inp = str;
    };       
    return inp;
  };

  string AKARE::encrypt(string inp, size_t &key) {
    if (inp.length() != bounds::expectedLength) {return "ERR INVALID LENGTH";};
    
    string result;
    key = evenGenerator();
    size_t realKey = mod(key / staticPrivateKey) + mod(key);
    
    srand(realKey);
    
    for (int i = 0; i < inp.length(); i+=bounds::bitsInByte) {
      size_t looper = rand() % 100 + 1;
      result += shuffle8(inp.substr(i, bounds::bitsInByte), looper);
    };
    
    return result;
  };

  string AKARE::decrypt(string inp, size_t key) {
    if (inp.length() != bounds::expectedLength) {return "ERR INVALID LENGTH";};
    
    string result;
    size_t realKey = mod(key / staticPrivateKey) + mod(key);
    
    srand(realKey);
    
    for (int i = 0; i < inp.length(); i+=bounds::bitsInByte) {
      size_t looper = rand() % 100 + 1;
      result += unshuffle8(inp.substr(i, bounds::bitsInByte), looper);
    };
    
    return result;
  };
};