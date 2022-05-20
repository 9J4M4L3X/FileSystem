#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

#include"Folder.h"

//contains the pointer to the tree of folders and files 
class FileSystem {
public:
    FileSystem() { root_ = current_directory_ = nullptr; }
    FileSystem(Folder* root, Folder* current_directory, string log_filepath) :
        root_(root), current_directory_(current_directory), log_filepath_(log_filepath) {};
    ~FileSystem();

    void loadHierarchy(const string& fs_filepath, const string& log_filepath);//reads the input and forms a tree of folders with files
    void execute(const string& filepath);//executes the (exe)file on location specified by filepath

private:
    Folder* root_, * current_directory_;
    string log_filepath_;
};
#endif // !_FILESYSTEM_H_

