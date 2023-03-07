#include "FileOP_includes.h"

namespace FileOperations {
  class FileOP {
    public:
    //Input dir - &files - &folders
    static void searchDir(char* directory, string &files, string &folders, bool doRecursiveSearch = false);

    static bool isRealDir(char* path);
    static bool isRealDir(string path);
    static bool mkFile(string path);
    static bool checkPath(string path);

    struct TXT {
      public:
        static constexpr short id_len = 6;
        static const string identifier[id_len];
        static void read(string path, string& data);
        static void write(string path, string data);
    };

    private:
    static int isFile(char* path);
  };
}