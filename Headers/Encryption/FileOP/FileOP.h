#include "FileOP_includes.h"

namespace LINUXFILEOP {
  class FileOP {
    public:
    //Input dir - &files - &folders
    static void searchDir(char* directory, string &files, string &folders, bool doRecursiveSearch = false);

    static bool isRealDir(char* path);
    static bool isRealDir(string path);

    private:
    static int isFile(char* path);
  };
}