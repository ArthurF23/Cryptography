#include "FileOP_includes.h"

#include "FileOP.h"
namespace FileOperations {
  const string FileOP::TXT::identifier[id_len] = {".txt", ".md", ".cpp", ".h", ".cs", ".c"};

  bool FileOP::checkPath(string path) {
    ifstream infile(path);
    if (!infile.good()) {infile.close();return false;} else{infile.close(); return true;};
  };

  int FileOP::isFile(char* path) {
    //Check path
    ifstream infile(path);
    if (!infile.good()) {infile.close();return -1;};
    infile.close();
    
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode); //true is file | false is dir
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

  void FileOP::TXT::read(string path, string& data) {
    ifstream infile(path, ios::binary);
    //get length of file:
    infile.seekg (0, infile.end);
    int length = infile.tellg();
    infile.seekg (0, infile.beg);

    //buffer for data
    char * buffer = new char [length];

    // read data as a block:
    infile.read(buffer,length);
    data += buffer;
    //Don't need this anymore sooo...
    delete[] buffer;
    infile.close();
  };

  void FileOP::TXT::write(string path, string data) {
    //Original File
    ofstream {path}; //Create... Doesn't matter if it's overwritten because it's about to be anyways
    ofstream outfile(path, ios::out | ios::trunc);
    outfile << data; //write to file
    outfile.close(); //close file
  };

  bool FileOP::mkFile(string path) {
    ofstream outFile(path);
    if (outFile.good() == false) {outFile.close(); return false;} else {return true;};
  };

};