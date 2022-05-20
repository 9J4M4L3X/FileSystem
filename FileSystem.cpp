#include "FileSystem.h"

FileSystem::~FileSystem()
{
	current_directory_ = nullptr;
	//delete the tree
	Folder* root_folder = root_;

	stack<Pair*> st;
	int currentRootIndex = 0;//start from the most left

	//traversing and deleting all folders and files from leafs to the root_folder 
	while (!st.empty() || root_folder != nullptr) {

		//root_folder will be nullptr only after we have reached the leaf
		if (root_folder != nullptr) {

			st.push(new Pair(root_folder, currentRootIndex));
			//save all nodes that are traversed on the stack, so they can get processed(deleted) later
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

void FileSystem::loadHierarchy(const string& fs_filepath, const string& log_filepath)
{
	//reading filesystem filepaths from a file
	ifstream input(fs_filepath);
	stringstream ss;
	ss << input.rdbuf(); input.close();
	string filepath1 = ss.str();

	//makes one base folder(root) with no name(if it already exists then delete the hierarchy)
	if (!root_) {
		root_ = new Folder();
		current_directory_ = root_;
	}
	else {
		//deleting the hierarchy->
		/************************/
		
		Folder* root_folder = root_;

		stack<Pair*> st;
		int currentRootIndex = 0;//start from the most left

		//traversing and deleting all folders and files from leafs to the root_folder 
		while (!st.empty() || root_folder != nullptr) {

			//root_folder will be nullptr only after we have reached the leaf
			if (root_folder != nullptr) {

				st.push(new Pair(root_folder, currentRootIndex));
				//save all nodes that are traversed on the stack, so they can get processed(deleted) later
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
		
		/***********************/
		
		root_ = new Folder();
		current_directory_ = root_;
	}

	bool end_of_string = false;
	unsigned int i = 0;
	
	//reads input and makes a tree hierarchy of folders and files
	try {
		while (i < filepath1.size())
		{
			while ((filepath1[i] == '\n') || (filepath1[i] == ' '))i++;//skips ' ' and '\n'

			Folder* node = root_;

			//next while makes reads one line in the file, and adds a file or a folder from the end to the hierarchy
			while (filepath1[i] != '\n')
			{
				if (filepath1[i] == '\\') i++;//skips '\'

				string name;
				bool txt = false, exe = false, folder = true;

				//reading folder/file name
				while (filepath1[i] != '\\')
				{
					if (filepath1[i] == '\n') break;
					//if there is a '.' in the name then it is a file 

					if (filepath1[i] == '.')
					{
						//this is a file
						folder = false;
						if (filepath1[i + 1] == 'e' && filepath1[i + 2] == 'x' && filepath1[i + 3] == 'e')
						{
							//it is an exe file
							exe = true;
							name.append(".exe");
							i += 4;
						}
						if (filepath1[i + 1] == 't' && filepath1[i + 2] == 'x' && filepath1[i + 3] == 't')
						{
							//it is a txt file
							txt = true;
							name.append(".txt");
							i += 4;//jumps behind .txt
						}

						if (i >= filepath1.size()) {
							end_of_string = true;
							break;
						}

						while (filepath1[i] == ' ') i++;//skips ' '

						if ((!exe && !txt) || (filepath1[i] != '\n')) {
							//throw an exception invalid file type
							throw new InvalidFileType();
						}
						break;
					}

					bool letter = ((filepath1[i] <= 'Z') && (filepath1[i] >= 'A')) || ((filepath1[i] <= 'z') && (filepath1[i] >= 'a'));
					bool number = (filepath1[i] <= '9') && (filepath1[i] >= 'A');
					bool underscore = filepath1[i] == '_';

					if (!letter && !number && !underscore) {
						//throw an exception invalid file/folder name(forbidden character in a name)
						throw InvalidName();
					}

					name.push_back(filepath1[i++]);

					if (i >= filepath1.size()) {
						end_of_string = true;
						break;
					}
				}

				//makes a file or a folder depending on the previous reading of input file
				if (!folder)
				{
					//makes exe file
					if (exe)
					{
						if (i >= filepath1.size()) break;

						while (filepath1[i] == ' ') i++;

						if (i >= filepath1.size()) break;

						string exe_text = "";
						while (filepath1[i] != '\n') {
							exe_text.push_back(filepath1[i++]);
							if (i >= filepath1.size()) {
								end_of_string = true;
								break;
							}
						}
						ExeFile* exe_file = new ExeFile();
						exe_file->setFileName(name);
						exe_file->setText(exe_text);
						exe_file->parseExeText(log_filepath_);//ovde izdvoji iz teksta listu komandi

						node->addFile(exe_file);
					}
					//makes txt file
					if (txt)
					{
						if (i >= filepath1.size()) break;

						while (filepath1[i] == ' ') i++;//skips blanks

						if (i >= filepath1.size()) break;

						string txt_text = "";
						while (filepath1[i] != '\n') {
							txt_text.push_back(filepath1[i++]);
							if (i >= filepath1.size()) {
								end_of_string = true;
								break;//we got to the end of the file
							}
						}
						TxtFile* txt_file = new TxtFile(txt_text);
						txt_file->setFileName(name);

						node->addFile(txt_file);
					}
				}
				else
				{
					//makes a folder and adds it to the file system
					Folder* folder = new Folder();
					folder->setName(name);

					//checkChild = -1 if the children folder does not exist, otherwise returns the position of that child
					int checkChild = node->childChecker(folder);

					//if the child is not added to the hierarchy here it is added->
					if (checkChild == -1)
					{
						node->addChild(folder);
						folder->setParent(node);
						node = folder;
					}
					//this means that the folder is already added, node is then set to that folder
					else
						node = node->getChild(checkChild);
				}

				if (end_of_string) break;//end_of_string is true if we reached the end of the file

			}
		}
	}
	catch (exception & e) {
		cout << e.what() << '\n';
	}
	log_filepath_ = log_filepath;
}

void FileSystem::execute(const string& filepath)
{
	ifstream input(filepath);
	stringstream ss;
	ss << input.rdbuf();
	input.close();
	string exe_text = ss.str();//text of the (txt)file we read

	File* exe_file = new ExeFile();
	exe_file->setText(exe_text);
	exe_file->parseExeText(log_filepath_);
	try {
		current_directory_ = exe_file->execute(current_directory_);//executes all commands in a file
	}
	catch (exception & e) {
		cout << '\n' << e.what() << '\n';
	}
}
