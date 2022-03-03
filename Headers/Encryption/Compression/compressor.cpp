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
  const string rgb_compression::firstLayerMulRChars[firstLayerMultiplier[0]][firstLayerMultiplier[1]] {
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
    {"10", "@"}
  };
  const string rgb_compression::sectionStart = "{";
  const string rgb_compression::sectionEnd = "}";

  void rgb_compression::get_chunk_count(unsigned long int &inp, string clone, char separator, bool div) {
    inp = count(clone.begin(), clone.end(), separator);
    //since each pixel is 3 values, divide by 3 to get actual chunk value
    if (div == true) {
      inp/=3;
    };
  };

  void rgb_compression::replaceSeparator(string &inp, char sep, char repl) {
    int len = inp.length()-1;
    for (int y = 0; y<len; y++) {
      if (inp[y] == sep) {inp[y] = repl;};
    };
  };

  bool rgb_compression::CORE::COMP::loop_params(unsigned int len, string cln, char sep) {
    if (len > sizeLimit) {
      unsigned long int cnt; rgb_compression::get_chunk_count(cnt, cln, sep, false);
      if (cnt%3 == 0) {return false;} else {return true;};
    };
    return true;
  };


  void rgb_compression::CORE::COMP::compress(string &inp, char separator) {
     string clone = inp;
    //count chunks
    unsigned long int chunk_count; get_chunk_count(chunk_count, clone, separator);
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
      for (; rep < firstLayerMultiplier[0]-1;) {
        if (rep > firstLayerMultiplier[0]-1) {break;};
        if (chunks[rep + i] == chunks[i]) {rep++;}
        else {break;};
      };
      
      //Do compression thing if repeat > 1
      if (rep > 1) {
        string temp;
        temp += sectionStart + chunks[i] + sectionEnd + firstLayerMulRChars[rep][1] + separator;
        //Do thing with commas to make code go brrrrr
        replaceSeparator(temp, separator, _separator); inp += temp; temp.clear();
        i += rep;
      }
      else {
        replaceSeparator(chunks[i], separator, _separator);
        inp += chunks[i];
        i++;
      };
    };
  };

  void rgb_compression::compress(string &inp, char separator) {
    //This is so it doesnt allocate too much memory at a time
    const unsigned int len = inp.length();
    if (len > CORE::COMP::sizeLimit) {
      string clone = inp, temp = inp; inp.clear();
      while (clone.length() > 0) {
        string section, section2;
        //////////
        //////////
        //////////
        for (;CORE::COMP::loop_params(section.length(), section, separator);) {
          if (CORE::COMP::loop_params(section.length(), section, separator)==false) {break;};
          if (clone.length() == 0) {break;};
          int first = clone.find_first_of(separator) + 1;
          section += clone.substr(0, first); clone.erase(0, first);
        };
        //////////
        //////////
        thread th1(CORE::COMP::compress, ref(section), separator);
        //////////
        //////////
        if (clone.length() == 0) {
          th1.join();
          inp+=section;
          break;
        };
        for (;CORE::COMP::loop_params(section2.length(), section2, separator);) {
          if (CORE::COMP::loop_params(section2.length(), section2, separator)==false) {break;};
          if (clone.length() == 0) {break;};
          int first = clone.find_first_of(separator) + 1;
          section2 += clone.substr(0, first); clone.erase(0, first);
        };
        
        thread th2(CORE::COMP::compress, ref(section2), separator);
        th1.join(); th2.join();
        inp+=section; inp+=section2;
      };
    } else {CORE::COMP::compress(inp, separator);};
  };

  /////////////////////////////
  /// Decompression //////////
  ///////////////////////////

  string rgb_compression::CORE::DECOMP::halfify(string &cln, char sep) {
    unsigned int len = cln.length();
    unsigned int param = (len/2)+10;
    int i;
    for (i=0; i<param; i++) {
      if (param-i < 10 && cln[i] == sep) {break;};
    }
    i++;
    string ret = cln.substr(0, i); 
    cln.erase(0, i);
    return ret;
  };

  void rgb_compression::CORE::DECOMP::decompress(string &inp, char separator) {
    //get all chunks
    unsigned long int chunk_count = 0; get_chunk_count(chunk_count, inp, separator, false);
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
      replaceSeparator(section, _separator, separator);
      //if first char is { then we know its a compressed chunk big brain
      if (section[0] == sectionStart[0]) {
        char mul = section[section.length()-2];
        section = section.substr(1, section.length()-4);
        int rep;
        //how many times to make repeat loop go brrrr
        for (rep = 0; rep<firstLayerMultiplier[0]; rep++) {
          if (firstLayerMulRChars[rep][1][0] == mul) {break;};
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

  void rgb_compression::decompress(string &inp, char separator) {
    string str1 = CORE::DECOMP::halfify(inp, separator);
    string str2 = inp; inp.clear();
    thread th(rgb_compression::CORE::DECOMP::decompress, ref(str1), separator);
    thread th2(rgb_compression::CORE::DECOMP::decompress, ref(str2), separator);
    th.join(); th2.join();
    
    string sbt = str2.substr(str2.length()-2, str2.length()-1);
    stringstream ss;
    ss << separator; ss << separator;
    
    if (sbt == ss.str()) {
      str2.erase(str2.length()-1, str2.length()-1);
    };
    inp+=str1+str2;    
  };

//Multithread example
/*int d = 2;
  thread th(&foo, ref(d)); //int& in func
  th.join();
  cout << d << endl;
  */
};