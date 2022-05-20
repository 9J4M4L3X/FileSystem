#ifndef _FOLDER_H_
#define _FOLDER_H_

#include"File.h"

//node in the file hierarchy, contains folders(children) and files 
class Folder {
public:
	Folder();
	Folder(vector<Folder*> children, vector<File*> files, string folder_name, Folder* parent) :
		children_(children), files_(files), folder_name_(folder_name), parent_(parent) {};
	~Folder();


	string getFolderName() const { return folder_name_; }
	Folder* getParent() const { return parent_; }
	vector<Folder*> getChildren() const { return children_; }
	Folder* getChild(int position) const { return children_.at(position); }
	vector<File*> getFiles() const { return files_; }
	File* getFile(int position) const { return files_.at(position); }
	int getNumOfChildren() const;
	int getNumOfFiles() const;
	
	void addChild(Folder* child);
	void addFile(File* file);
	void setParent(Folder* parent) { parent_ = parent; }
	void setChildren(vector<Folder*> children) { children_ = children; }
	void setFiles(vector<File*> files) { files_ = files; }
	void setName(string folder_name) { folder_name_ = folder_name; }
	
	void removeFile(int position);//removes a file thats determined by an integer 'position' in a list of files
	void removeAllFiles() { files_.clear(); }
	void removeFolder(int position);//removes a child thats on the position determined by an integer position

	bool checkUnvisitedChildren(Folder* child);//returns true if there are more children after "child" in an array children_
	int childChecker(Folder* child);//returns -1 if child is not among children_, otherwise returns the position of that child
	int fileChecker(File* file);//returns -1 if file is not among files_, otherwise returns the position of that file
protected:
	string folder_name_;
	Folder* parent_;
	vector<Folder*> children_;//an array of folders that inside this folder
	vector<File*> files_;//an array of files inside this folder
};

//helper class for postorder traversal
class Pair {
public:
	Pair(Folder* folder, int children_index) :folder_(folder), children_index_(children_index) {};

	Folder* getFolder() { return folder_;}
	int getChildIndex() { return children_index_; }
private:
	Folder* folder_;
	int children_index_;//index of this child
};

#endif // !_FOLDER_H_
