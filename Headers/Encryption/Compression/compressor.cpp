#include "compression_includes.h"

#include "compressor.h"
namespace COMPRESSION {
  //Roughly ~20% compression ratio on the bee movie script, my accurate representation of a lot of data. Th original binary length was 437376 and was compressed to 328840 characters long.
  string binary_compression::compress(string input) {    
    string str;

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
    }
    return str + breakChar;
  };

  string binary_compression::decompress(string input) {
    string output;

    for (int i = 0; i < input.length(); i++) {
      if (input[i] == breakChar) {break;};
      if (input[i+1] != '0' && input[i+1] != '1' && input[i+1] != breakChar) {
        for (int x = 0; x < input[i+1]-'0'; x++) {
          output += input[i];
        };
        i++;
      } else {output+=input[i];};
    };
    return output;
  };
};