#include<exception>

using namespace std;
	 
class FileNotExecutable :public exception {
public:
	FileNotExecutable() : exception("Error:File is not executable!") {};
	FileNotExecutable(char* t) : exception(t) {};
};

class InvalidExeName :public exception {
public:
	InvalidExeName() :exception("Error:No exe file named like that in the current directory!") {};
	InvalidExeName(char* t) :exception(t) {};
};

class InvalidName :public exception {
public:
	InvalidName() :exception("Error:Folder or a file has an invalid character in its name!") {};
	InvalidName(char* t) :exception(t) {};
};

class InvalidFileType :public exception {
public:
	InvalidFileType() :exception("Error:Invalid file type!") {};
	InvalidFileType(char* t) :exception(t) {};
};

class FileDoesNotExist :public exception {
public:
	FileDoesNotExist() :exception("Error:File with the specified name does not exist!") {};
	FileDoesNotExist(char* t) :exception(t) {};
};

class FolderDoesNotExist :public exception {
public:
	FolderDoesNotExist() :exception("Error:Folder with the specified name does not exist!") {};
	FolderDoesNotExist(char* t) :exception(t) {};
};

class FolderPassedToExe :public exception {
public:
	FolderPassedToExe() :exception("Error:EXE command called for a directory!") {};
	FolderPassedToExe(char* t) :exception(t) {};
};