#include "compression_includes.h"

#include "compressor.h"
namespace COMPRESSION {
  const string binary_compression::replacementChars[rCharLength[0]][rCharLength[1]] = {
    {"01", "A"},
    {"02", "B"},
    {"03", "C"},
    {"04", "D"},
    {"05", "E"},
    {"06", "F"},
    {"07", "G"},
    {"08", "H"},
    {"09", "I"},

    {"10", "J"},
    {"11", "K"},
    {"12", "L"},
    {"13", "M"},
    {"14", "N"},
    {"15", "O"},
    {"16", "P"},
    {"17", "Q"},
    {"18", "R"},
    {"19", "S"},

    {"20", "T"},
    {"21", "a"},
    {"22", "b"},
    {"23", "c"},
    {"24", "d"},
    {"25", "e"},
    {"26", "f"},
    {"27", "g"},
    {"28", "h"},
    {"29", "i"},
    {"30", "U"},
    {"31", "j"},
    {"32", "k"},
    {"40", "V"},
    {"41", "l"},
    {"42", "m"},
    {"50", "W"},
    {"51", "n"},
    {"52", "o"},
    {"60", "X"},
    {"61", "p"},
    {"62", "q"},
    {"70", "Y"},
    {"71", "r"},
    {"72", "s"},
    {"80", "Z"},
    {"81", "t"},
    {"82", "u"},
    {"90", "@"},
    {"91", "v"},
    {"92", "w"}
    };

  const string binary_compression::thirdLayerMulRChars[thirdLayerMultiplier[0]][thirdLayerMultiplier[1]] = {
    {"0", "."},
    {"1", ","},
    {"2", "}"},
    {"3", "]"},
    {"4", "["},
    {"5", "{"},
    {"6", "%"},
    {"7", "*"},
    {"8", "&"},
    {"9", "$"}
    };
  //These are what ive found to be the most common 2 characters
  const string binary_compression::fourthLayerMulRChars[fourthLayerMultiplier[0]][fourthLayerMultiplier[1]] = {
    {"A}", "x"},
    {"Ta", "y"},
    {"BL", "z"},
    {"BJ", ","},
    {"AT", "."},
    {"CJ", ":"},
    {"LA", "("},
    {"AU", ")"},
    {"TJ", "|"},
    {"AV", "'"},
    {"jT", "/"},
    {"Ua", "\\"},
    {"Uj", "+"},
    {"UA", "-"},
    {"MA", "_"},
    {"BM", "~"},
    {"UJ", "`"},
    {"Ja", "<"},
    {"Dj", ">"},
    {"DJ", "#"},
    {"CL", "="},
    {"TL", "!"},
    {"NA", "^"},
    {"aA", "\""},
    {"aT", "€"},
    {"JL", " "},
    {"Ul", "\2"},
    {"Vj", "\1"},
    {"UL", "\7"},
    {"TM", "\13"},
    {"EJ", "\16"},
    {"Xj", "\17"},
    {"Vl", "\6"},
    {"VL", "\27"}
    };


  const string binary_compression::fifthLayerMulRChars[fifthLayerMultiplier[0]][fifthLayerMultiplier[1]] = {
    {"0", ""},
    {"1", ""},
    {"2", ""},
    {"3", "\3"},
    {"4", "\4"},
    {"5", "\5"}
    };

  //So far the compressed size is 24.575% of the original size.

  void binary_compression::compress(string& input) {    
    string str;

    //First compression
    //Say you have 2 bytes 0100001100111100
    //That would be reduced to 010412021402
    //01 is the same, then 04 means repeat 0 four times in decompression, 12 = 11 etc....
    for (int i = 0, increment = 0; i < input.length(); i++) {
      for (increment = 1;; increment++) {
        if (input[i] != input[i+increment] || increment == 9) {break;};
      };
      if (increment > 1) {
      str+=input[i] + to_string(increment); i += increment-1;
      }
      else {
        str += input[i];
      };
      increment = 1;      
    };

    input = str;
    str.clear();

    //Second compression of the compressed stuff already
    for (int i = 0; i<input.length(); i++) {
      string twoBytes;

      if (i+1 == input.length()) {str+=input[i]; break;};
      
      for (int x = 0; x < 2; x++) {
        twoBytes+=input[i+x];
      };
      
      for (int r = 0; r < rCharLength[0]; r++) {
        if (replacementChars[r][0] == twoBytes) {
          str+=replacementChars[r][1];
          i++;
          break;
        }
        else if (r+1 == rCharLength[0]) {
          str+=input[i];
        }
      }
    };

    input = str;
    str.clear();
    
    //Third layer of compression
    for (int i = 0, increment = 1; i < input.length(); i++) {
      for (increment = 1;; increment++) {
        if (input[i] != input[i+increment] || increment == 9) {break;};
      };
      
      if (increment > 1) {
        str+=input[i] + thirdLayerMulRChars[increment][1];
        i += increment-1;
      }
      else {
        str += input[i];
      };
      increment = 1;      
    };


    input = str;
    str.clear();

    //Fourth Layer
    for (int i = 0; i<input.length(); i++) {
      string twoBytes;

      if (i+1 == input.length()) {str+=input[i]; break;};
      
      for (int x = 0; x < 2; x++) {
        twoBytes+=input[i+x];
      };
      
      for (int r = 0; r < fourthLayerMultiplier[0]; r++) {
        if (fourthLayerMulRChars[r][0] == twoBytes) {
          str+=fourthLayerMulRChars[r][1];
          i++;
          break;
        }
        else if (r+1 == fourthLayerMultiplier[0]) {
          str+=input[i];
        }
      }
    };

    input = str;
    str.clear();
    
    //Fifth layer of compression
    for (int i = 0, increment = 1; i < input.length(); i++) {
      for (increment = 1;; increment++) {
        if (input[i] != input[i+increment] || increment == 5) {break;};
      };
      
      if (increment > 2) {
        str+=input[i] + fifthLayerMulRChars[increment][1];
        i += increment-1;
      }
      else {
        str += input[i];
      };
      increment = 1;      
    };

    str+=breakChar;
    input = str; str.clear();
  };

  void binary_compression::decompress(string& input) {
    string output;
    //Fifth Layer Decompression
    for (int i = 0, pos = 0; i < input.length(); i++) {
      if (input[i] == breakChar) {output+=breakChar; break;}
      for (int x = 0; x < fifthLayerMultiplier[0]; x++) {
        if (input[i] == (fifthLayerMulRChars[x][1][0])) {pos = x; break;};
      };
      if (pos > 2) {
        int rep = fifthLayerMulRChars[pos][0][0]-'0';
        for (int x = 0; x < rep-1; x++) {
          output+=input[i-1];
        };
      }
      else {
        output+=input[i];
      };
      pos=0;
    };
    input = output;
    output.clear();
    //Fourth layer decompression
    for (int i = 0; i < input.length(); i++) {
      if (input[i] == breakChar) {output+=breakChar; break;};
      
      for (int x = 0; x < fourthLayerMultiplier[0]; x++) {
        if (fourthLayerMulRChars[x][1][0] == input[i]) {
          output+=fourthLayerMulRChars[x][0];
          break;
        }
        else if (x+1 == fourthLayerMultiplier[0]) {
          output+=input[i];
        };
      };
    };
    
    input=output;
    output.clear();

    //Third Layer Decompression
    for (int i = 0, pos = 0; i < input.length(); i++) {
      if (input[i] == breakChar) {output+=breakChar; break;}
      for (int x = 0; x < thirdLayerMultiplier[0]; x++) {
        if (input[i] == (thirdLayerMulRChars[x][1][0])) {pos = x; break;};
      };
      if (pos > 1) {
        int rep = thirdLayerMulRChars[pos][0][0]-'0';
        for (int x = 0; x < rep-1; x++) {
          output+=input[i-1];
        };
      }
      else {
        output+=input[i];
      };
      pos=0;
    };
    input = output;
    output.clear();

    //Second layer decompression
    for (int i = 0; i < input.length(); i++) {
      if (input[i] == breakChar) {output+=breakChar; break;};
      
      for (int x = 0; x < rCharLength[0]; x++) {
        if (replacementChars[x][1][0] == input[i]) {
          output+=replacementChars[x][0];
          break;
        }
        else if (x+1 == rCharLength[0]) {
          output+=input[i];
        };
      };
    };
    
    input=output;
    output.clear();

    //Binary decompression
    for (int i = 0; i < input.length(); i++) {
      if (input[i] == breakChar) {break;};
      if (input[i+1] != '0' && input[i+1] != '1' && input[i+1] != breakChar) {
        for (int x = 0; x < input[i+1]-'0'; x++) {
          output += input[i];
        };
        i++;
      } else {output+=input[i];};
    };
    input = output; output.clear();
  };

  /////////////////////////
  ///  RGB  ///////////////
  /////////////////////////
  const string rgb_compression::CORE::VAR::firstLayerMulRChars[firstLayerMultiplier[0]][firstLayerMultiplier[1]] {
    {"0", ""}, //Will not be used
    {"1", ""}, // ^
    {"2", "|"},
    {"3", "$"},
    {"4", "'"},
    {"5", "]"},
    {"6", "["},
    {"7", "="},
    {"8", "+"},
    {"9", "-"},
    {"10", "@"},
    {"11", "!"},
    {"12", ">"},
    {"13", "<"},
    {"14", "/"},
    {"15", "?"},
    {"16", "_"},
    {"17", "\""},
    {"18", "\\"},
    {"19", "a"},
    {"20", "b"},
    {"21", "c"},
    {"22", "d"},
    {"23", "e"},
    {"24", "f"},
    {"25", "g"},
    {"26", "h"},
    {"27", "i"},
    {"28", "j"},
    {"29", "k"},
    {"30", "l"},
    {"31", "m"},
    {"32", "n"},
    {"33", "o"},
    {"34", "p"},
    {"35", "q"},
    {"36", "r"},
    {"37", "s"},
    {"38", "t"},
    {"39", "u"},
    {"40", "v"},
    {"41", "w"},
    {"42", "x"},
    {"43", "y"},
    {"44", "z"},
    {"45", "A"},
    {"46", "B"},
    {"47", "C"},
    {"48", "D"},
    {"49", "E"},
    {"50", "F"},
    {"51", "G"},
    {"52", "H"},
    {"53", "I"},
    {"54", "J"},
    {"55", "K"},
    {"56", "L"},
    {"57", "M"},
    {"58", "N"},
    {"59", "O"},
    {"60", "P"},
    {"61", "Q"},
    {"62", "R"},
    {"63", "S"},
    {"64", "T"},
    {"65", "U"},
    {"66", "V"},
    {"67", "W"},
    {"68", "X"},
    {"69", "Y"},
    {"70", "Z"},
    {"71", "€"},
    {"72", "\2"},
    {"73", "\1"},
    {"74", "\7"},
    {"75", "\13"},
    {"76", "\16"},
    {"77", "\17"},
    {"78", "\6"},
    {"79", "\27"}
};

  void rgb_compression::CORE::FUNC::get_chunk_count(unsigned long int &inp, string clone, char separator, bool div) {
    inp = count(clone.begin(), clone.end(), separator);
    //since each pixel is 3 values, divide by 3 to get actual chunk value
    if (div == true) {
      inp/=3;
    };
  };

  void rgb_compression::CORE::FUNC::replaceSeparator(string &inp, char sep, char repl) {
    int len = inp.length()-1;
    for (int y = 0; y<len; y++) {
      if (inp[y] == sep) {inp[y] = repl;};
    };
  };

  void rgb_compression::CORE::COMP::compress(string &inp, char separator) {
    unique_ptr<rgb_compression::CORE::FUNC> func;
    unique_ptr<rgb_compression::CORE::VAR> var;
    string clone = inp;
    //count chunks
    unsigned long int chunk_count; func->get_chunk_count(chunk_count, clone, separator);
    string chunks[chunk_count]; //Big memory allocation

    //assign chunks to arr
    for (int i=0; i<chunk_count && clone.length() != 0; i++) {
      //Three at a time since a chunk consists of 3 values like so
      //255,98,23,
      if (clone.length() == 0) {break;};
      int first = clone.find_first_of(separator) + 1;
      chunks[i] += func->hexify(stoi(clone.substr(0, first-1)))/* + separator*/;
      clone.erase(0, first);

      if (clone.length() == 0) {break;};
      first = clone.find_first_of(separator) + 1;
      chunks[i] += func->hexify(stoi(clone.substr(0, first-1)))/* + separator*/;
      clone.erase(0, first);
      
      if (clone.length() == 0) {break;};
      first = clone.find_first_of(separator) + 1;
      chunks[i] += func->hexify(stoi(clone.substr(0, first-1))) + separator;
      clone.erase(0, first);
      if (clone[0] == ' ') {break;};
    };
    //Free memory
    inp.clear(); clone.clear();
    

    //////                           //////
    ///////////////////////////////////////
    /// This is where the magic happens ///
    ///////////////////////////////////////
    //////                           //////

    
    //Find matching chunks and pair them accordingly
    for (unsigned long int i = 0; i<chunk_count;) {
      unsigned long int rep = 0;
      
      //Find repeating chunks
      for (; rep < var->firstLayerMultiplier[0]-1;) {
        if (rep > var->firstLayerMultiplier[0]-1) {break;};
        if (chunks[rep + i] == chunks[i]) {rep++;}
        else {break;};
      };
      
      //Do compression thing if repeat > 1
      if (rep > 1) {
        string temp;
        temp += var->sectionStart + chunks[i] + var->sectionEnd + var->firstLayerMulRChars[rep][1] + separator;
        //Do thing with commas to make code go brrrrr
        func->replaceSeparator(temp, separator, var->_separator);
        inp += temp; temp.clear();
        i += rep;
      }
      else {
        func->replaceSeparator(chunks[i], separator, var->_separator);
        inp += chunks[i];
        i++;
      };
    };
  };

  void rgb_compression::compress(string &inp, char separator) {
    unique_ptr<CORE::COMP> comp;
    
    if (inp.length() > comp->sizeLimit) {
      unique_ptr<CORE::FUNC> func;
      
      //Chop up input into halfs
      string half1 = func->halfify(inp, separator);
      string half2 = inp; inp.clear();
      
      string quarter1 = func->halfify(half1, separator);
      string quarter2 = half1; half1.clear();
      
      //quarters to eights and launch threads
      string eighth1 = func->halfify(quarter1, separator);
      thread th1(comp->compress, ref(eighth1), separator);
      
      string eighth2 = quarter1; quarter1.clear();
      thread th2(comp->compress, ref(eighth2), separator);
      
      string eighth3 = func->halfify(quarter2, separator);
      thread th3(comp->compress, ref(eighth3), separator);
      
      string eighth4 = quarter2; quarter2.clear();
      thread th4(comp->compress, ref(eighth4), separator);

      ////////////////
      string quarter3 = func->halfify(half2, separator);      
      string quarter4 = half2; half2.clear();
      ////////////////
      
      string eighth5 = func->halfify(quarter3, separator);
      thread th5(comp->compress, ref(eighth5), separator);
      
      string eighth6 = quarter3; quarter3.clear();
      thread th6(comp->compress, ref(eighth6), separator);
      
      string eighth7 = func->halfify(quarter4, separator); func.reset();
      thread th7(comp->compress, ref(eighth7), separator);
      
      string eighth8 = quarter4; quarter4.clear();
      thread th8(comp->compress, ref(eighth8), separator);
      
      th1.join(); th2.join(); th3.join(); th4.join();
      th5.join(); th6.join(); th7.join(); th8.join();
      
      inp+=eighth1;
      inp+=eighth2;
      inp+=eighth3;
      inp+=eighth4;   
      inp+=eighth5;
      inp+=eighth6;
      inp+=eighth7;
      inp+=eighth8;
      
    } else {comp->compress(inp, separator);}; comp.reset();
    
  };

  /////////////////////////////
  /// Decompression //////////
  ///////////////////////////

  string rgb_compression::CORE::FUNC::halfify(string &cln, char sep) {
    unsigned int len = cln.length();
    unsigned int param = (len/2)+10;
    int i;
    for (i=0; i<len; i++) {
      if (param-i < 20 && cln[i] == sep) {break;}
    }
    i++;
    string ret = cln.substr(0, i);
    cln.erase(0, i);
    return ret;
  };

  void rgb_compression::CORE::DECOMP::decompress(string &inp, char separator, string id) {
    unique_ptr<rgb_compression::CORE::FUNC> func;
    unique_ptr<rgb_compression::CORE::VAR> var;
    //get all chunks
    unsigned long int chunk_count = 0; func->get_chunk_count(chunk_count, inp, separator, false);
    string clone = inp;
    string chunks[chunk_count];
    //assign chunks to arr
    for (int i=0; i<chunk_count; i++) {
      int first = clone.find_first_of(separator) + 1;
      chunks[i] += clone.substr(0, first);  clone.erase(0, first);
    };
    clone.clear(); inp.clear();
    
    
    //////                           //////
    ///////////////////////////////////////
    /// This is where the magic happens ///
    ///////////////////////////////////////
    //////                           //////

    
    for (int i=0; i<chunk_count; i++) {
      string str = chunks[i];
      
      string mulChar;
      bool isCompressed = false;
      if (str[0] == var->sectionStart) {
        isCompressed=true;
        str.erase(0,1);
        mulChar = str[str.length()-2];
        str.erase(str.length()-4, 4); 
        };
      
      string str1, str2, str3;
      str1 = str.substr(0,2); str.erase(0,2);
      str2 = str.substr(0,2); str.erase(0,2);
      str3 = str.substr(0,2); str.erase(0,2);
      func->dehexify(str1); func->dehexify(str2); func->dehexify(str3);
      str.clear();

      //rebuild chunk
      if (isCompressed) {
        str+=var->sectionStart;
      };
      str+=str1+var->_separator+str2+var->_separator+str3;
      if (isCompressed) {
        str+=var->_separator;
        str+=var->sectionEnd;
        str+=mulChar;
        str+=separator;
      } else {
        str+=separator;
      };
      chunks[i]=str;
    };
    
    for (int i=0; i<chunk_count; i++) {
      string section = chunks[i];
      if (corruptChunk(section, separator) == false) {
        CORE::FUNC::replaceSeparator(section, CORE::VAR::_separator, separator);
        //if first char is { then we know its a compressed chunk big brain
        if (section[0] == CORE::VAR::sectionStart) {
          char mul = section[section.length()-2];
          section = section.substr(1, section.length()-4);
          int rep;
          //how many times to make repeat loop go brrrr
          for (rep = 0; rep<CORE::VAR::firstLayerMultiplier[0]; rep++) {
            if (CORE::VAR::firstLayerMulRChars[rep][1][0] == mul) {break;};
          };
          //Repeat chunk to str
          for (int x=0; x<rep; x++) {
            inp+=section;
          };
        }
        //Pretty self explanatory
        else {
          inp+=section;
        };
      };
    };
    errorChecker(inp, separator);
  };

  void rgb_compression::decompress(string &inp, char separator) {
    unique_ptr<CORE::FUNC> func;
    unique_ptr<CORE::DECOMP> decomp;
    //Chop up input into halfs
    string half1 = func->halfify(inp, separator);
    string half2 = inp; inp.clear();
    
    string quarter1 = func->halfify(half1, separator);
    string quarter2 = half1; half1.clear();
    
    //quarters to eights and launch threads
    string eighth1 = func->halfify(quarter1, separator);
    thread th1(decomp->decompress, ref(eighth1), separator, "1");
    
    string eighth2 = quarter1; quarter1.clear();
    thread th2(decomp->decompress, ref(eighth2), separator, "2");
    
    string eighth3 = func->halfify(quarter2, separator);
    thread th3(decomp->decompress, ref(eighth3), separator, "3");
    
    string eighth4 = quarter2; quarter2.clear();
    thread th4(decomp->decompress, ref(eighth4), separator, "4");

    ////////////////
    string quarter3 = func->halfify(half2, separator);      
    string quarter4 = half2; half2.clear();
    ////////////////
    
    string eighth5 = func->halfify(quarter3, separator);
    thread th5(decomp->decompress, ref(eighth5), separator, "5");
    
    string eighth6 = quarter3; quarter3.clear();
    thread th6(decomp->decompress, ref(eighth6), separator, "6");
    
    string eighth7 = func->halfify(quarter4, separator); func.reset();
    thread th7(decomp->decompress, ref(eighth7), separator, "7");
    
    string eighth8 = quarter4; quarter4.clear();
    thread th8(decomp->decompress, ref(eighth8), separator, "8");

    
    th1.join(); th2.join(); th3.join(); th4.join();
    th5.join(); th6.join(); th7.join(); th8.join();
    
    inp+=eighth1;
    inp+=eighth2;
    inp+=eighth3;
    inp+=eighth4;   
    inp+=eighth5;
    inp+=eighth6;
    inp+=eighth7;
    inp+=eighth8;
    
    decomp->errorChecker(inp, separator); decomp.reset();
  };

  void rgb_compression::CORE::DECOMP::errorChecker(string &cln, char sep) {
    constexpr short offset = 2;
    //check for repeating commas
    for (int i=0; i<cln.length(); i++) {
      if (cln[i] == sep) {
        if (cln[i+1] == sep) {
          cln.erase(i, 1); i-=offset;
        };
      };

      //remove invalid characters
      bool isGood = false;      
      for (int x=0; x<vCharLen; x++) {  
        if (cln[i] == validChars[x] || cln[i] == sep) {
          isGood = true; break;    
        };
      };
      
      if (isGood == false) {
        cln.erase(i, 1); i-=offset;
      };
    };
  };

  bool rgb_compression::CORE::DECOMP::corruptChunk(string cnk, char sep) {
    bool containsEndRep = false;
    bool containsStartRep = false;
    
    if (cnk[0] == CORE::VAR::sectionStart) {containsStartRep = true;};
    if (cnk[cnk.length()-3] == CORE::VAR::sectionEnd) {containsEndRep =  true;};
    
    //Is it a repeated chunk?
    if (containsStartRep != containsEndRep) {return true;};
    
    //Does it end in a sep
    if (cnk[cnk.length()-1] != sep) {return true;};
    
    return false;
  };

  //////////////////////////
  // ~ Pixel assignment ~ //
  //////////////////////////
  void rgb_compression::asgnPix(unsigned char* &PIX, string data, char sep, size_t length) {
    //Pointers for ease of reading and writing
    unique_ptr<CORE::FUNC> func;
    unique_ptr<CORE::PIXELS> pixels;
    
    //Allocate memory
    char* pix = (char*)malloc(length); 
    PIX = (unsigned char*)malloc(length);
    
    //Figure out how to split input down into managable, threadable forms
    //Will be 8 threads
    
    //Half
    string str1 = func->halfify(data, sep);
    string str2 = data; data.clear();

    //Quarter
    string quar1 = func->halfify(str1, sep);
    string quar2 = str1; str1.clear();
    string quar3 = func->halfify(str2, sep);
    string quar4 = str2; str2.clear();

    //Eighth
    string eth1 = func->halfify(quar1, sep);
    uInt _eth1[2] = {0, pixels->findLength(eth1, sep)};
    
    string eth2 = quar1; quar1.clear();
    uInt _eth2[2] = {_eth1[1], pixels->findLength(eth2, sep)+_eth1[1]};
    
    string eth3 = func->halfify(quar2, sep);
    uInt _eth3[2] = {_eth2[1], pixels->findLength(eth3, sep)+_eth2[1]};
    
    string eth4 = quar2; quar2.clear();
    uInt _eth4[2] = {_eth3[1], pixels->findLength(eth4, sep)+_eth3[1]};
    
    string eth5 = func->halfify(quar3, sep);
    uInt _eth5[2] = {_eth4[1], pixels->findLength(eth5, sep)+_eth4[1]};
    
    string eth6 = quar3; quar3.clear();
    uInt _eth6[2] = {_eth5[1], pixels->findLength(eth6, sep)+_eth5[1]};
    
    string eth7 = func->halfify(quar4, sep);
    uInt _eth7[2] = {_eth6[1], pixels->findLength(eth7, sep)+_eth6[1]};
    
    string eth8 = quar4; quar4.clear();
    uInt _eth8[2] = {_eth7[1], pixels->findLength(eth8, sep)+_eth7[1]};
    
    thread asgn1(pixels->asgnPixThr, ref(pix), eth1, sep, _eth1[0], _eth1[1]);
    thread asgn2(pixels->asgnPixThr, ref(pix), eth2, sep, _eth2[0], _eth2[1]);
    thread asgn3(pixels->asgnPixThr, ref(pix), eth3, sep, _eth3[0], _eth3[1]);
    thread asgn4(pixels->asgnPixThr, ref(pix), eth4, sep, _eth4[0], _eth4[1]);
    thread asgn5(pixels->asgnPixThr, ref(pix), eth5, sep, _eth5[0], _eth5[1]);
    thread asgn6(pixels->asgnPixThr, ref(pix), eth6, sep, _eth6[0], _eth6[1]);
    thread asgn7(pixels->asgnPixThr, ref(pix), eth7, sep, _eth7[0], _eth7[1]);
    thread asgn8(pixels->asgnPixThr, ref(pix), eth8, sep, _eth8[0], _eth8[1]);
    
    pixels.reset(); func.reset(); //free mem taken by smart pointers
    
    asgn1.join();
    asgn2.join();
    asgn3.join();
    asgn4.join();
    asgn5.join();
    asgn6.join();
    asgn7.join();
    asgn8.join();
    
    PIX = reinterpret_cast<unsigned char*>(pix);
  };

  uInt rgb_compression::CORE::PIXELS::findLength(string str, char sep) {
    uInt num = 0;
    for (int i=0; i<str.length(); i++) {
      if (str[i] == sep) {
        num++;
      };
    };
    return num;
  };

  void rgb_compression::CORE::PIXELS::asgnPixThr(char* &pix, string data, char sep, uInt startPos, uInt endPos) {
    for (unsigned int i=startPos; i<endPos+1; i++) {
      //substring rgb num
      if (data == "") {break;};
      size_t pos = data.find_first_of(sep);      
      if (pos == string::npos) {break;}
      string sm = data.substr(0, pos);
      if (sm == "") {break;};
      
      //assign
      char uiui = stoi(sm);      
      pix[i] = uiui;
      
      //delete from data
      if (data.find(sep) != string::npos && pos < data.length()) {
        data.erase(0, data.find_first_of(sep)+1);
      } else {break;};
    };
  };

  //HEX COMPRESSION
  string rgb_compression::CORE::FUNC::hexify(unsigned short inp) {
    unique_ptr<rgb_compression::CORE::FUNC> func;
    unique_ptr<rgb_compression::CORE::VAR> var;
    float num = inp; num/=var->hexSize;
    //Remove trailing 0s
    stringstream ss;
    ss << setprecision(4) << num;
    string result;
    string nu;
    ss >> nu;
    string fnum, snum; //first and second number

    bool sec = false;
    for (int i=0; i<nu.length(); i++){
      if (nu[i] == '.') {sec=true;};
      if (!sec) {fnum+=nu[i];}
      else if (sec && nu[i]!='.') {snum+=nu[i]; if(snum.length()==2){break;};};
    };
    nu.clear();
    
    float ssnum = 0; 
    if (snum.length() > 0) {
      ssnum = stof(snum) * .01; 
      ssnum *= var->hexSize;
    };
    int ffnum = stoi(fnum); int sssnum = ssnum;
    result = var->hexArr[ffnum]; result += var->hexArr[sssnum];
    return result;
  };

  void rgb_compression::CORE::FUNC::dehexify(string &inp) {
    unique_ptr<rgb_compression::CORE::VAR> var;
    string result;
    int res1 = 0; int res2 = 0;
    for (short i=0; i<var->hexSize; i++) {
      if (inp[0] == var->hexArr[i]) {res1=i*var->hexSize;};
      if (inp[1] == var->hexArr[i]) {res2=i;};
    };
    res1+=res2;
    result = to_string(res1);
    inp=result;
  };

//Multithread example
  /*
  int ex = 2;
  thread th(&foo, ref(ex)); //int& in func
  th.join();
  */
};