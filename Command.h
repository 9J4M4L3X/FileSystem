#ifndef _COMMAND_H_
#define _COMMAND_H_

#include<fstream>
#include<iostream>
#include<list>
#include<sstream>
#include<stack>
#include<string>
#include<vector>
#include"Exceptions.h"

using namespace std;

class Folder;
class File;
class ExeFile;
class TxtFile;

class Command {
public:
	virtual Folder* executeCommand(Folder* curr_directory) = 0;

	void setLogPath(string log_filepath) { log_filepath_ = log_filepath; }
	string getLogPath() { return log_filepath_; }
protected:
	string log_filepath_;
};

//writes in a log file all the directories inside the current directory and all their children, with all their files
class Ls :public Command {
public:
	virtual Folder* executeCommand(Folder* curr_directory) override;
};

//changes directory to the one inside the current directory or to the parent directory
class Cd :public Command {
public:
	Cd() { new_curr_directory_name_ = ""; }
	Cd(string new_curr_directory_name) :new_curr_directory_name_(new_curr_directory_name) {};
	
	virtual Folder* executeCommand(Folder* curr_directory) override;
private:
	string new_curr_directory_name_;
};

//makes a new directory or a file inside the current directory
class New :public Command {
public:
	New() { new_directory_name_ = file_text_ = ""; file_ = false; }
	New(string new_directory_name, string file_text, bool file) 
		:new_directory_name_(new_directory_name), file_text_(file_text), file_(file) {}
	
	virtual Folder* executeCommand(Folder* curr_directory) override;
private:
	string new_directory_name_;//new directory or a file name
	string file_text_;
	bool file_;
};

//deletes a folder or a file in the hierarchy
class Del :public Command {
public:
	Del() { delete_directory_name_ = ""; file_ = false; }
	Del(string delete_directory_name, bool file) :delete_directory_name_(delete_directory_name), file_(file) {};
	
	virtual Folder* executeCommand(Folder* curr_directory) override;
private:
	bool file_;//if it is true then it is deleting a file, otherwise it deletes a folder
	string delete_directory_name_;//directory(file) name
};

//executes a file
class Exe :public Command {
public:
	Exe() { exe_file_name_ = ""; }
	Exe(string exe_file_name) : exe_file_name_(exe_file_name) {};

	virtual Folder* executeCommand(Folder* curr_directory) override;

	void setFile(string exe_file_name) { exe_file_name_ = exe_file_name; }
private:
	string exe_file_name_;//file to be executed
};
#endif // !_COMMAND_H_
