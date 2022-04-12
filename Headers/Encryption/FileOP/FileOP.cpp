#include "FileOP_includes.h"

#include "FileOP.h"
namespace LINUXFILEOP {

  int FileOP::isFile(char* path) {
    //Check path
    ifstream infile(path);
    if (!infile.good()) {infile.close();return -1;};
    infile.close();
    
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode); //true is file false is dir
  };

  void FileOP::searchDir(char* path, string &files, string &folders, bool doRecursiveSearch) {
    DIR *dir;
    struct dirent *ent;
    
    if ((dir = opendir(path)) != NULL) {
      /* print all the files and directories within directory */
      while ((ent = readdir (dir)) != NULL) {
        string p = path; p+=ent->d_name;
        char* newpath = const_cast<char*>(p.c_str());
        
        if (isFile(newpath) != -1) { 
          //Sort 
          if (isFile(newpath) == 1) { //Files
            files += static_cast<string>(newpath) + '\n';
          } else {
              folders += static_cast<string>(newpath) + '\n';
              if (doRecursiveSearch == true) {
                p = newpath;
                p+="/";
                newpath = const_cast<char*>(p.c_str());
                FileOP::searchDir(newpath, files, folders);
              };
            }; //Folders
        };
      }
      closedir (dir);
    } else {
      /* could not open directory */
      files = "Could Not Open Dir\n" + static_cast<string>(path); 
      folders = "Could Not Open Dir\n" + static_cast<string>(path);
    }
  };

  bool FileOP::isRealDir(char* path) {
    ifstream file(path);
    return file.good(); file.close();
  };

  bool FileOP::isRealDir(string path) {
    ifstream file(path);
    return file.good(); file.close();
  };
};