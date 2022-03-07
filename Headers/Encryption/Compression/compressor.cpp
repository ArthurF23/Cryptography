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
    {"15", "!"},
    {"20", ">"},
    {"30", "<"},
    {"40", "/"},
    {"50", "?"},
    {"100", "_"},
    {"1000", "\""},
    {"5000", "\\"},
    {"10000", "a"},
    {"100000", "b"},
    {"1000000", "c"}
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
     string clone = inp;
    //count chunks
    unsigned long int chunk_count; CORE::FUNC::get_chunk_count(chunk_count, clone, separator);
    string chunks[chunk_count]; //Big memory allocation

    //assign chunks to arr
    for (int i=0; i<chunk_count && clone.length() != 0; i++) {
      //Three at a time since a chunk consists of 3 values like so
      //255,98,23,
      if (clone.length() == 0) {break;};
      int first = clone.find_first_of(separator) + 1;
      chunks[i] += clone.substr(0, first);  clone.erase(0, first);

      if (clone.length() == 0) {break;};
      first = clone.find_first_of(separator) + 1;
      chunks[i] += clone.substr(0, first);  clone.erase(0, first);
      
      if (clone.length() == 0) {break;};
      first = clone.find_first_of(separator) + 1;
      chunks[i] += clone.substr(0, first);  clone.erase(0, first);
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
      for (; rep < CORE::VAR::firstLayerMultiplier[0]-1;) {
        if (rep > CORE::VAR::firstLayerMultiplier[0]-1) {break;};
        if (chunks[rep + i] == chunks[i]) {rep++;}
        else {break;};
      };
      
      //Do compression thing if repeat > 1
      if (rep > 1) {
        string temp;
        temp += CORE::VAR::sectionStart + chunks[i] + CORE::VAR::sectionEnd + CORE::VAR::firstLayerMulRChars[rep][1] + separator;
        //Do thing with commas to make code go brrrrr
        CORE::FUNC::replaceSeparator(temp, separator, CORE::VAR::_separator); inp += temp; temp.clear();
        i += rep;
      }
      else {
        CORE::FUNC::replaceSeparator(chunks[i], separator, CORE::VAR::_separator);
        inp += chunks[i];
        i++;
      };
    };
  };

  void rgb_compression::compress(string &inp, char separator) {
    if (inp.length() > CORE::COMP::sizeLimit) {
      //Chop up input into halfs
      string half1 = CORE::FUNC::halfify(inp, separator);
      string half2 = inp; inp.clear();
      
      string quarter1 = CORE::FUNC::halfify(half1, separator);
      string quarter2 = half1; half1.clear();
      
      //quarters to eights and launch threads
      string eighth1 = CORE::FUNC::halfify(quarter1, separator);
      thread th1(CORE::COMP::compress, ref(eighth1), separator);
      
      string eighth2 = quarter1; quarter1.clear();
      thread th2(CORE::COMP::compress, ref(eighth2), separator);
      
      string eighth3 = CORE::FUNC::halfify(quarter2, separator);
      thread th3(CORE::COMP::compress, ref(eighth3), separator);
      
      string eighth4 = quarter2; quarter2.clear();
      thread th4(CORE::COMP::compress, ref(eighth4), separator);

      ////////////////
      string quarter3 = CORE::FUNC::halfify(half2, separator);      
      string quarter4 = half2; half2.clear();
      ////////////////
      
      string eighth5 = CORE::FUNC::halfify(quarter3, separator);
      thread th5(CORE::COMP::compress, ref(eighth5), separator);
      
      string eighth6 = quarter3; quarter3.clear();
      thread th6(CORE::COMP::compress, ref(eighth6), separator);
      
      string eighth7 = CORE::FUNC::halfify(quarter4, separator);
      thread th7(CORE::COMP::compress, ref(eighth7), separator);
      
      string eighth8 = quarter4; quarter4.clear();
      thread th8(CORE::COMP::compress, ref(eighth8), separator);

      
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
      
    } else {CORE::COMP::compress(inp, separator);};
  };

  /////////////////////////////
  /// Decompression //////////
  ///////////////////////////

  string rgb_compression::CORE::FUNC::halfify(string &cln, char sep) {
    unsigned int len = cln.length();
    unsigned int param = (len/2)+10;
    int i;
    for (i=0; i<len; i++) {
      if (param-i < 10 && cln[i] == sep) {break;};
    }
    i++;
    string ret = cln.substr(0, i); 
    cln.erase(0, i);
    return ret;
  };

  void rgb_compression::CORE::DECOMP::decompress(string &inp, char separator) {
    //get all chunks
    unsigned long int chunk_count = 0; CORE::FUNC::get_chunk_count(chunk_count, inp, separator, false);
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
    //Chop up input into halfs
    string half1 = CORE::FUNC::halfify(inp, separator);
    string half2 = inp; inp.clear();
    
    string quarter1 = CORE::FUNC::halfify(half1, separator);
    string quarter2 = half1; half1.clear();
    
    //quarters to eights and launch threads
    string eighth1 = CORE::FUNC::halfify(quarter1, separator);
    thread th1(CORE::DECOMP::decompress, ref(eighth1), separator);
    
    string eighth2 = quarter1; quarter1.clear();
    thread th2(CORE::DECOMP::decompress, ref(eighth2), separator);
    
    string eighth3 = CORE::FUNC::halfify(quarter2, separator);
    thread th3(CORE::DECOMP::decompress, ref(eighth3), separator);
    
    string eighth4 = quarter2; quarter2.clear();
    thread th4(CORE::DECOMP::decompress, ref(eighth4), separator);

    ////////////////
    string quarter3 = CORE::FUNC::halfify(half2, separator);      
    string quarter4 = half2; half2.clear();
    ////////////////
    
    string eighth5 = CORE::FUNC::halfify(quarter3, separator);
    thread th5(CORE::DECOMP::decompress, ref(eighth5), separator);
    
    string eighth6 = quarter3; quarter3.clear();
    thread th6(CORE::DECOMP::decompress, ref(eighth6), separator);
    
    string eighth7 = CORE::FUNC::halfify(quarter4, separator);
    thread th7(CORE::DECOMP::decompress, ref(eighth7), separator);
    
    string eighth8 = quarter4; quarter4.clear();
    thread th8(CORE::DECOMP::decompress, ref(eighth8), separator);

    
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
    
    CORE::DECOMP::errorChecker(inp, separator);
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

//Multithread example
/*int d = 2;
  thread th(&foo, ref(d)); //int& in func
  th.join();
  cout << d << endl;
  */
};