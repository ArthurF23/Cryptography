#include "PNG_Includes.h"

#include "PNG.h"
namespace PNG_H {
  const string PNG::ID = ".png";

  bool PNG::read(string path, string& data) {
    //Prepare data var to have the buffer applied to it
    data.clear();
    data = "";

    //Init file thing
    ifstream infile(path, std::ios::in | std::ios::binary | std::ios::ate);
    
    //get length of file:
    infile.seekg (0, infile.end);
    int length = infile.tellg();
    infile.seekg (0, infile.beg);

    //buffer for data
    char * buffer = new char [length];

    // read data as a block:
    infile.read(buffer,length);

    //After read close
    infile.close();
    
    //Assign it to the data var
    for (int i=0; i<length; i++) {
      data += buffer[i];
    };
    
    //Don't need this anymore sooo...
    delete[] buffer;
    
    return true;
  };

  bool PNG::write(string path, string data) {
    ofstream out(path); out << data; out.close(); //One line func lol
    return true;
  };
};