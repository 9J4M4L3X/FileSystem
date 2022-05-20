#include "Command.h"
#include "File.h"
#include"Folder.h"

Folder* Ls::executeCommand(Folder* curr_directory)
{	
	ofstream log_file;
	log_file.open(log_filepath_, ios::out);
	//file open in output mode

	stack<Folder*> unvisitedFolders;
	unvisitedFolders.push(curr_directory);

	int counter = 1, num_of_paths = 20;
	string* folder_paths = new string[num_of_paths];//these are all paths that include only folders(without files added)
	folder_paths[0] = "";//this just marks the root folder name(curr_directory) for simplification afterwards
	vector<string> filepaths;//an array of all paths possible that are starting from current directory(files included)

	//traversing the tree(preorder) and writing all possible paths in an array filepaths
	while (!unvisitedFolders.empty())
	{
		Folder* curr_folder = unvisitedFolders.top();
		unvisitedFolders.pop();
		
		//appending folder name to the filepath and all its files and pushing children folders on the stack
		if (curr_folder != nullptr) 
		{
			string previous_path = "";
			//no need to append "\root_name", and there is no previous path if we are at the root folder
			if (curr_folder != curr_directory) {
				
				//if the curr_folder is a leaf then find the parent with kids to know where to continue the next path
				if (curr_folder->getNumOfChildren() == 0) {
					
					Folder* curr_folder_helper = curr_folder;//saving current position
					
					int folders_without_children_counter = 0;//counts how many times we decrement the pointer
					//searches for the parent with kids so it can continue the new path there afterwards
					
					while (!(curr_folder->getParent()->checkUnvisitedChildren(curr_folder)) && (curr_folder->getParent() != nullptr)) {
						counter--;//in each iteration goes one folder up(like it pops one folder name from the string)
						folders_without_children_counter++;
						curr_folder = curr_folder->getParent();//go up in the tree to find a parent which has another kid
						if (curr_folder->getParent() == nullptr) break;
					}
					
					//counter--;//one more place up because while jumps out one iteration before we reach the path
					
					previous_path = folder_paths[counter];//saves the previous path to return to after adding the leaf
					
					//folders_without_children_counter++;//same reasoning as it is for counter
					counter += folders_without_children_counter;//goes back to the current path
					curr_folder = curr_folder_helper;//go back to the current folder
					
				}			
				string new_folder = '\\' + curr_folder->getFolderName();
				folder_paths[counter] += new_folder;//append the new_folder to the existing path
				filepaths.push_back(folder_paths[counter]);//makes the "base" path(without files)
			}

			vector<File*> curr_files = curr_folder->getFiles();
			vector<File*>::iterator file_iterator = curr_files.begin();

			//push current path and file names at the end of the string array(filepaths) for each file in the curr_folder
			while (file_iterator != curr_files.end()) 
			{
				//appends to the path all the files finishing that path->
				if(curr_folder != curr_directory)
					filepaths.push_back(folder_paths[counter] + '\\' + (*file_iterator)->getFileName());
				else
					filepaths.push_back('\\' + (*file_iterator)->getFileName());
				
				file_iterator++;
			}

			int curr_children_number = curr_folder->getNumOfChildren();
			//if it gets to the leaf move the path to another branch
			if (curr_children_number == 0) {
				counter++; 
				folder_paths[counter] = previous_path;//returns to the path that has the next children folder
				//alocates more space if needed
				if (counter > num_of_paths - 5) {
					num_of_paths += 20;
					string* allocate_here = new string[num_of_paths];
					int counter_for_realloc = 0;
					while (counter_for_realloc <= counter) {
						allocate_here[counter_for_realloc] = folder_paths[counter_for_realloc];
						counter_for_realloc++;
					}
					delete[] folder_paths;
					folder_paths = allocate_here;
				}
			}

			//push all children in a node(right to left) to the stack		
			while (curr_children_number > 0)
			{
				unvisitedFolders.push(curr_folder->getChild(curr_children_number - 1));
				curr_children_number--;
			}
		}
	}
	
	//write all paths(from the current_directory) in separate rows to log_file 
	vector<string>::iterator iter = filepaths.begin();
	while (iter != filepaths.end()) {
		if (iter + 1 != filepaths.end())
			log_file << *iter << '\n';
		else 
			log_file << *iter;
		iter++;
	}

	log_file.close();

	return curr_directory;
}

Folder* Cd::executeCommand(Folder* curr_directory)
{
	if (new_curr_directory_name_ == "..") {
		if(curr_directory->getParent())
			curr_directory = curr_directory->getParent();
		return curr_directory;
	}

	vector<Folder*> children = curr_directory->getChildren();
	vector<Folder*>::iterator children_it = children.begin();
	
	while (children_it != children.end()) {
		if ((*children_it)->getFolderName() == new_curr_directory_name_) {
			return (*children_it);
		}
		children_it++;
	}
	return curr_directory;//if the folder name is not correct the current directory stays the same
}

Folder* New::executeCommand(Folder* curr_directory)
{
	//adds a new folder or a file to the current directory
	if (!file_) {
		//adds a folder
		Folder* new_folder = new Folder();
		new_folder->setName(new_directory_name_);
		curr_directory->addChild(new_folder);
		new_folder->setParent(curr_directory);
		return curr_directory;
	}
	else {
		//adds a file
		File* new_file = nullptr;
		
		int counter = 0;
		while (new_directory_name_[counter] != '.') counter++;
		counter++;//moves to the character after '.'

		if ((new_directory_name_[counter] == 'e') && (new_directory_name_[counter + 1] == 'x') && (new_directory_name_[counter + 2] == 'e'))
		{
			new_file = new ExeFile();
			new_file->setFileName(new_directory_name_);
			new_file->setText(file_text_);
			new_file->parseExeText(this->getLogPath());
		}
		if ((new_directory_name_[counter] == 't') && (new_directory_name_[counter + 1] == 'x') && (new_directory_name_[counter + 2] == 't')) 
		{
			new_file = new TxtFile;
			new_file->setFileName(new_directory_name_);
			new_file->setText(file_text_);
		}
		
		if (counter + 3 <= new_directory_name_.size()) {
			//this means there are characters after "exe" so its not a valid sufix
			//throw an exception: Not a valid file type
			throw new InvalidFileType();
		}

		if (new_file) curr_directory->addFile(new_file);
		else {
			//throw an exception->invalid file type
			throw new InvalidFileType();
		}

		return curr_directory;
	}
}

Folder* Del::executeCommand(Folder* curr_directory)
{
	//deletes one folder(and all its subfolders and files) from the current directory
	int counter = 0;
	bool file = false, exe = false, txt = false;
	while (counter < delete_directory_name_.size()) {
		if (delete_directory_name_[counter++] == '.') {
			file = true;
			txt = (delete_directory_name_[counter] == 't') && 
				(delete_directory_name_[counter + 1] == 'x') && (delete_directory_name_[counter + 2] == 't');
			exe = (delete_directory_name_[counter] == 'e') &&
				(delete_directory_name_[counter + 1] == 'x') && (delete_directory_name_[counter + 2] == 'e');
			if (counter + 3 <= delete_directory_name_.size()) {
				//this means that the sufix has more characters after "exe" or "txt"
				throw new InvalidFileType();
			}
		}
	}

	if (file) {
		File* file_to_remove = nullptr;
		if(exe)	file_to_remove = new ExeFile();
		if(txt) file_to_remove = new TxtFile();


		if (file_to_remove) {
			file_to_remove->setFileName(delete_directory_name_);

			int file_check = curr_directory->fileChecker(file_to_remove);

			if (file_check == -1) {
				//FILE WITH THAT NAME IS NOT IN THE CURRENT DIRECTORY
				throw new FileDoesNotExist();
			}
			else {
				curr_directory->removeFile(file_check);
			}
		}
		else {
			throw new InvalidFileType();
		}
	}
	else //its a folder name so it has to delete all its subfolders
	{
		Folder* folder_to_remove = new Folder();
		folder_to_remove->setName(this->delete_directory_name_);
		int folder_check = curr_directory->childChecker(folder_to_remove);
		//folder_check is either the index of a child that will be deleted or 
		//it is -1 which means that there is no child with that name(no such folder in the current directory)

		if (folder_check == -1) {
			//throw an exception: folder with that name does not exist in the current directory 
			throw new FolderDoesNotExist();
		}
		else {
			//traverse the tree in postorder, it starts deleting from the leafs upward

			Folder* root_folder = curr_directory->getChild(folder_check);

			stack<Pair*> st;
			int currentRootIndex = 0;//start from the most left

			//traversing and deleting all folders and files from leafs to the root_folder 
			while (!st.empty() || root_folder != nullptr) {
				
				//root_folder will be nullptr only after we have reached the leaf
				if (root_folder != nullptr) {

					st.push(new Pair(root_folder, currentRootIndex));//save all nodes that are traversed so they can get processed later
					currentRootIndex = 0;

					if (root_folder->getNumOfChildren() >= 1) {
						root_folder = root_folder->getChild(0);//sets it to its most left child
					}
					else {
						//here i should save the leaf i think

						root_folder = nullptr;//if root has no children(leaf) then we can proceed -> from the leaf go up, or right
					}
					continue;
				}

				Pair* temp = st.top();//tops the leaf folder 
				st.pop();

				//the next while block will only be done if the top folder on the stack is the last child of the parent folder
				while (st.size() > 0 && temp->getChildIndex() == st.top()->getFolder()->getNumOfChildren() - 1) {
					//temp is the most right folder that is a leaf so it can be deleted-> 
					temp->getFolder()->removeAllFiles();//removes all the files in the leaf folder
					temp->getFolder()->getParent()->removeFolder(temp->getChildIndex());//removes the leaf folder from the hierarchy
					temp = st.top();//set the temp to the parent
					st.pop();//take that parent of the stack
				}

				//if the top node on the stack(parent) has more children we set the root to be the next child of the parent node
				if (st.size() > 0) {
					//update the root to be the next child of the top node on stack
					root_folder = st.top()->getFolder()->getChild(temp->getChildIndex() + 1);
					currentRootIndex = temp->getChildIndex() + 1;//updates root index to be of the pushed child			
				}
			}
		}
	}
	return curr_directory;
}

Folder* Exe::executeCommand(Folder* curr_directory)
{
	vector<File*> curr_folder_files = curr_directory->getFiles();
	vector<File*>::iterator file_iterator = curr_folder_files.begin();

	while (file_iterator != curr_folder_files.end()) {
		if ((*file_iterator)->getFileName() == exe_file_name_) {
			curr_directory = (*file_iterator)->execute(curr_directory);
			return curr_directory;
		}
		file_iterator++;
	}

	return curr_directory;
}
