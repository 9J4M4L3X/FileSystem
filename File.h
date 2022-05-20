#ifndef _FILE_H_
#define _FILE_H_

#include"Command.h"

//abstract class, has a filepath for output and file name as class members
class File {
public:
	File() { file_name_ = ""; }
	File(string file_name) : file_name_(file_name) {};

	string getFileName() { return file_name_; }
	virtual string getText() const = 0;
	
	virtual void setFileName(string) = 0;
	virtual void setText(string text) = 0;
	virtual void parseExeText(string log_filepath) = 0;//takes log_file path as argument and sets it to LS command for output
	virtual Folder* execute(Folder* curr_directory) = 0;
protected:
	string log_filepath_;
	string file_name_;
};

//executable file
class ExeFile :public File {
public:
	ExeFile(list<Command*> lst_com, string text) : com_list_(lst_com), text_(text) {};
	ExeFile(string text, list<Command*> com_list) :text_(text), com_list_(com_list) {}
	ExeFile() { text_ = ""; com_list_.clear(); }
	~ExeFile() { com_list_.clear(); }

	string getText() const override { return text_; }
	list<Command*>getExeCommands() { return com_list_; }
	
	void setFileName(string exe_name) override { file_name_ = exe_name; }
	void setText(string text) override { text_ = text; }
	void addCommand(Command* command) { com_list_.push_back(command); }

	void parseExeText(string log_filepath) override;//reads the exe text and makes a list of commands 
	Folder* execute(Folder* curr_directory) override;//executes a file 

private:
	string text_;
	list<Command*> com_list_;
};

//file with text 
class TxtFile :public File {
public:
	TxtFile() { text_ = ""; }
	TxtFile(string txt) :text_(txt) {};

	string getText() const override { return text_; }
	
	Folder* execute(Folder* curr_dir) override { return curr_dir; }
	void setText(string text) override { text_ = text; }
	void parseExeText(string log_filepath) override {};
	void addText(string additional_text) { text_.append(additional_text); }
	void setFileName(string txt_name) { file_name_ = txt_name; }

private:
	string text_;
};

#endif // !_FILE_H_
