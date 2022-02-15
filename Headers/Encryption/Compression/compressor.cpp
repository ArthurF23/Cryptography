#include "compression_includes.h"

#include "compressor.h"
namespace COMPRESSION {

  /*
        | This is a chunk  |
  length:charNum{positions}:etc...
  */


  string compression::compress(string input) {    
    map<char, int> table;
    for (int i = 0; i < input.length(); i++) {
      cout << i << " | " << input.length() << endl;
      char ch = input[i];
      table[input[i]] = std::count_if(input.begin(), input.end(), [&ch](char c) {
        return c == ch;
      });
    }
    
    string str;

    str += to_string(input.length()) + separator1;

    for(auto const& letter : table){
      // first == char, second == frequency 
      cout << letter.first << " | " << letter.second << endl;
      str+=to_string(char(letter.first)) + startPosSeparator + findOccOf(letter.first, input) + stopPosSeparator + separator1;
    };
    return str;
  };

  string compression::findOccOf(char search, string str) {
    string output;
    for (unsigned int i = 0; i < str.length(); i++) {
      if (str[i] == search) {
        if (output.length() == 0) {
          output+=to_string(i);
        } else {
          output+=occSeparator+to_string(i);
        };
      };
    };
    return output + occSeparator;
  };

  string compression::decompress(string input) {
    string output;
    
    //Find length
    unsigned int length = stoi(input.substr(0, input.find_first_of(separator1)));
    //Erase length from string
    input.erase(0, input.find_first_of(separator1)+1);

    //add spaces to output that equals the length of the original string
    for (int i = 0; i < length; i++) {
      output+=" ";
    };
    int lastPos = 0;
    for (unsigned int i = 0; i < input.length(); i++) {
      if (input[i] == separator1) {
        //Get chunk
        string chunk = input.substr(lastPos, i-lastPos);
        lastPos = i+1;
        //Break down data
        //Get startPosSeparator position... find_first_of stopped working for some reason....
        int startPos = 0;
        for (int l = 0; l < chunk.length(); l++) {
          if (chunk[l] == startPosSeparator) {
            startPos = l;
            break;
          };
        };
        string positions = chunk.substr(startPos+1, chunk.length());
        positions.erase(positions.length()-1, positions.length());
        chunk.erase(startPos, chunk.length());
        char letter = stoi(chunk);
        chunk.erase();

        unsigned int p = 0;
        while (positions.length() != 0) {
          p = positions.find_first_of(occSeparator);
          int POS = stoi(positions.substr(0, p));
          output[POS] = letter;
          positions.erase(0, p+1);
        };
      };
    };
    return output;
  };
};