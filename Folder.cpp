#include "Command.h"
#include "File.h"
#include"Folder.h"

Folder::Folder()
{
	children_.clear();
	files_.clear();
	parent_ = nullptr;
	folder_name_ = "";
}

Folder::~Folder()
{
	delete parent_;
	children_.clear();
	files_.clear();
	folder_name_ = "";
}

int Folder::getNumOfChildren() const
{
	return children_.size();
}

int Folder::getNumOfFiles() const
{
	return files_.size();
}

void Folder::addChild(Folder* child) 
{
	children_.push_back(child);
}

void Folder::addFile(File* file) 
{
	files_.push_back(file);
}

bool Folder::checkUnvisitedChildren(Folder* child)
{
	vector<Folder*>::iterator child_iter = children_.begin();
	while (child_iter != children_.end()) {
		if ((*child_iter)->getFolderName() == child->getFolderName()) {
			if (child_iter + 1 != children_.end()) {
				//if its not the last child in an array then we have more unvisited children to the right
				return true;
			}
		}
		child_iter++;
	}
	return false;
}

int Folder::childChecker(Folder* child)
{
	string child_name = child->getFolderName();
	int position = 0;
	for (vector<Folder*>::iterator it = children_.begin(); it != children_.end(); it++)
	{
		if ((*it)->getFolderName() == child_name) return position;
		position++;
	}
	return -1;
}

int Folder::fileChecker(File* file) {
	string file_name = file->getFileName();
	int position = 0;
	for (vector<File*>::iterator it = files_.begin(); it != files_.end(); it++)
	{
		if ((*it)->getFileName() == file->getFileName())
			return position;
		position++;
	}
	return -1;
}

void Folder::removeFile(int position) {
	files_.erase(files_.begin() + position);
}

void Folder::removeFolder(int position) {
	children_.erase(children_.begin() + position);
}
