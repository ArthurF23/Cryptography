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
    sMod(inp);
    srand(inp);
    size_t val = rand();
    while (1) {
      if (!isValidNum(val)) {val = rand();}
      else {break;};
    };
    return val;
  };

  //Makes mod() seed for srand more random with a custom algorithm
  void AKARE::sMod(size_t &inp) {
    srand(inp);
    if (inp > bounds::genMin*2) {inp/=2;};
    
    stringstream ss;
    string str;
    ss << inp;
    ss >> str;
    
    if (str[0]-'0' > 5) {inp*=4; inp+=rand();}; 
    
    ss.str("");
    ss << inp;
    ss >> str;
    ss.str("");
    
    if (str[2] < 5) {inp*=6;} else {inp*=8;};
    
    srand(inp);
    size_t ra = rand() % 100 + 1;
    
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
    
    size_t ra2 = rand() % 100 + 1, ra3 = rand() % 100 + 1;

    if (ra < ra2 && ra2 > ra3) {inp/=2;}
    else if (ra2 < ra && ra3 > ra) {inp*=2;}
    else if (ra < 70 && ra2 < 50 && ra3 < 60) {inp*=6;}
    else {inp*=8;};
    
    srand (ra+ra2+ra3+inp);
    if ((rand() % 100 + 1) >= 50) {inp/=2;} else {inp*=4;};

    ra = rand() % 2 + 1; ra--;

    if (ra == 1) {inp*=2;}
    else {inp/=2;};
    
    srand(inp);
    
    ra = fmod(rand(), rand());
    ra2 = fmod(ra, rand());
    ra3 = fmod(ra, ra2);
    
    srand(ra3);
    
    inp += ra + ra2 + ra3 + rand() + rand() + rand();

    srand(rand() + rand() + rand() + ra3 + rand());

    ra = rand() % 100 + 1;
    ra2 = pow(ra3, ra);
    inp += ra + ra2;

    srand(rand()+ra+fmod(ra,ra3)+rand()+rand()+inp+ra2+rand());

    ra = rand() % 1000 + 1;
    for (int i = 0; i < ra; i++) {
      ra3 = fmod(rand(), fmod(rand(), rand()));
      ra2 = fmod(fmod(rand(), rand()), fmod(fmod(rand(), rand()), fmod(rand(), rand())));
      inp -= fmod(ra3, ra2) + ra;
    };
    srand(fmod(rand(), rand()));
    ra = rand() % 100 + 1;

    if (ra > 50) {sMod(inp);};

    ra = rand() % 100 + 1;

    if (ra < 50) {inp -= fmod(rand(), rand());};
    if (ra < 25) {inp -= (fmod(rand(), rand()) * fmod(rand(), ra2));};
    if (ra > 75) {inp-=rand()*rand()*rand();};
    ra2 = rand() % 100 + 1;

    for (int i = 0; i < ra*ra2; i++) {
      srand(ra+ra2+ra3+inp+i+rand()+(rand()*rand()));
      srand(staticPrivateKey * rand() * rand() * rand());
      srand((rand() * rand()) + ra2 - (rand() * ra3) * rand());
      inp -= rand();
    };

    srand(staticPrivateKey+inp+rand()+rand()+ra+rand()+ra2+rand()+ra3);
    ra=rand()%1000+1; ra2=rand()%100+1; ra3=rand()%10+1;

    for (int i = 0; i < ra + ra2 * ra3; i++) {
      srand(rand()+staticPrivateKey+rand()*fmod(rand(),rand()));
      inp-=staticPrivateKey/(rand() % 100 + 2);
    };
    
    ra = fmod(rand(), rand());
    inp-=ra;

    for (int i = 0; i < (rand()%100+1); i++) {
      srand(fmod(rand(), rand()));
      inp-=rand();
    };

    srand(staticPrivateKey*rand()+rand()*rand());
    ra = rand();
    ra *= rand() + staticPrivateKey;

    inp -= ra;

    srand(rand()+rand()+rand()+ra+ra2+ra3+rand()*inp);

    ra = rand();
    ra2 = rand();
    ra3 = rand();

    ra += ra2*ra3;

    ra *= (ra2+ra3) * (ra3*rand()) + rand() * ra2 + ra3 * rand();

    if (ra > rand()*rand()) {
      ra*=rand();
    } else {ra -= (rand()*rand());};

    srand(ra);

    ra = rand() % 100 + 1;

    for (int i = 0; i < ra; i++) {
      srand(rand()+staticPrivateKey+inp);
      if (i%2==0) {inp+=rand()+staticPrivateKey;}
      else {inp-=rand();};
    };

    ss << inp;
    ss >> str;
    ss.str("");
    
    srand(fmod(inp, staticPrivateKey));
    ra = rand() % 100 + 1;
    
    inp+=str.length();
    
    for (int i = 0; i < ra; i++) {
      srand(fmod(rand(), rand()) + staticPrivateKey);
      short n = rand() % str.length() + 1;
      inp -= (str.length() * (str[n]-'0')) - rand();
    };
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
    sMod(realKey);
    
    srand(realKey);
    
    for (unsigned int i = 0; i < inp.length(); i+=bounds::bitsInByte) {
      unsigned short looper = rand() % 100 + 1;
      result += shuffle8(inp.substr(i, bounds::bitsInByte), looper);
    };
    
    return result;
  };

  string AKARE::decrypt(string inp, size_t key) {
    if (inp.length() != bounds::expectedLength) {return "ERR INVALID LENGTH";};
    
    string result;
    size_t realKey = mod(key / staticPrivateKey) + mod(key);
    sMod(realKey);
    
    srand(realKey);
    
    for (unsigned int i = 0; i < inp.length(); i+=bounds::bitsInByte) {
      unsigned short looper = rand() % 100 + 1;
      result += unshuffle8(inp.substr(i, bounds::bitsInByte), looper);
    };
    
    return result;
  };
};