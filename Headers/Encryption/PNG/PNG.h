#pragma once
#include "PNG_Includes.h"
namespace PNG_H {
  class PNG {
    public:
    static const string ID;
    static bool read(string path, string& data);
    static bool write(string path, string data);
  };
};