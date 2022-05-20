#include "Command.h"
#include "File.h"
#include"Folder.h"

void ExeFile::parseExeText(string log_filepath)
{
	bool end_of_string = false;
	unsigned counter = 0;

	while (counter < text_.size()) {

		if ((text_[counter] == 'L') && (text_[counter + 1] == 'S')) {

			Command* ls_command = new Ls();
			ls_command->setLogPath(log_filepath);

			this->addCommand(ls_command);

			counter += 2;
			//if we read all text break
			if (counter >= text_.size()) {
				end_of_string = true;
				break;
			}
			while ((text_[counter] == ' ') || (text_[counter] == '\n')) counter++;//skips newline and space
			//if there is a newline in a file we have a new command ahead
			if ((text_[counter] == '\\') && (text_[counter + 1] == 'n')) counter += 2;
		}
	
		if ((text_[counter] == 'C') && (text_[counter + 1] == 'D')) 
		{
			counter += 2;
			while ((text_[counter] == ' ') || (text_[counter] == '\n')) counter++;
			
			string new_folder_name;
			
			//"\n" is a sign we got to the end of the reading, if it is loading from load_file, but otherwise it can have '\n' as break
			while ((text_[counter] != '\\') || (text_[counter + 1] != 'n')) {
				
				bool letter = ((text_[counter] <= 'Z') && (text_[counter] >= 'A')) || ((text_[counter] <= 'z') && (text_[counter] >= 'a'));
				bool number = (text_[counter] <= '9') && (text_[counter] >= 'A');
				bool underscore = text_[counter] == '_';

				if (!letter && !number && !underscore) {
					//throw an exception invalid file/folder name(forbidden character in a name)
					throw new InvalidName();
				}

				new_folder_name.push_back(text_[counter++]);
				
				if (counter >= text_.size()) {
					end_of_string = true;
					break;
				}

				if (text_[counter] == '\n') break;
			}
			
			Command* cd_command = new Cd(new_folder_name);
			cd_command->setLogPath(log_filepath);

			this->addCommand(cd_command);

			if (end_of_string) break;
			if ((text_[counter] == '\\') && (text_[counter + 1] == 'n')) counter += 2;
		}

		if ((text_[counter] == 'N') && (text_[counter + 1] == 'E') && (text_[counter + 2] == 'W')) 
		{
			//this command has the name of the file as parameter but also its content
			counter += 3;
			while ((text_[counter] == ' ') || (text_[counter] == '\n')) counter++;//skips blank and skips newline 
			
			string new_folder_name, file_content;
			bool file = false, exe = false, txt = false;

			//reads the name of the file/folder
			while (text_[counter] != ' ') {
				
				bool letter = ((text_[counter] <= 'Z') && (text_[counter] >= 'A')) || ((text_[counter] <= 'z') && (text_[counter] >= 'a'));
				bool number = (text_[counter] <= '9') && (text_[counter] >= 'A');
				bool underscore = text_[counter] == '_';

				if (!letter && !number && !underscore) {
					//throw an exception invalid file/folder name(forbidden character in a name)
					throw new InvalidName();
				}

				new_folder_name.push_back(text_[counter++]);
				//checks if it is a file
				if (text_[counter] == '.') {
					exe = (text_[counter + 1] == 'e') && (text_[counter + 2] == 'x') && (text_[counter + 3] == 'e');
					txt = (text_[counter + 1] == 't') && (text_[counter + 2] == 'x') && (text_[counter + 3] == 't');
					if (!exe && !txt) {
						//throw an exception --> invalid file type;
						throw new InvalidFileType();
					}
					file = true;//this is a file so we set the file flag to be true
					counter += 4;//moves to the character after the postfix
					if (text_[counter] != ' ') {
						//throw an exception invalid file type
						throw new InvalidFileType();
					}
				}
				//if it is a folder or a file without content the newline can be right behind the last char of the name->
				if ((text_[counter] == '\\') && (text_[counter + 1] == 'n')) break;
				if (text_[counter] == '\n') break;
			}
				
			//if file is being created, read the content of the file
			if(file) 
			{
				if (exe) new_folder_name.append(".exe");
				if (txt) new_folder_name.append(".txt");
					
				while ((text_[counter] == ' ') || (text_[counter] == '\n')) counter++;//skips blank and newline characters
					
				//reads file content
				while ((text_[counter] != '\\') || (text_[counter + 1] != 'n')) 
				{
					file_content.push_back(text_[counter++]);

					if (text_[counter] == '\n') break;
					//next condition is if we reached the end of exe file text
					if (counter >= text_.size()) {
						end_of_string = true;
						break;
					}
				}
			}

			Command* new_command = new New(new_folder_name, file_content, file);
			new_command->setLogPath(log_filepath);

			this->addCommand(new_command);
			if (end_of_string) break;
			if ((text_[counter] == '\\') && (text_[counter + 1] == 'n')) counter += 2;
		}

		if ((text_[counter] == 'D') && (text_[counter + 1] == 'E') && (text_[counter + 2] == 'L')) 
		{
			counter += 3;//skips "DEL"
			while ((text_[counter] == ' ') || (text_[counter] == '\n')) 
				counter++;//skips '\n' or ' '
			
			string del_folder_name;
			bool file = false;
			
			while ((text_[counter] != '\\') || (text_[counter + 1] != 'n')) 
			{
				if (text_[counter] == '.') {
					bool exe = (text_[counter + 1] == 'e') && (text_[counter + 2] == 'x') && (text_[counter + 3] == 'e');
					bool txt = (text_[counter + 1] == 't') && (text_[counter + 2] == 'x') && (text_[counter + 3] == 't');
					if (!exe && !txt) {
						//throw an exception --> invalid file type;
						throw new InvalidFileType();
					}
					file = true;
				}
				
				bool letter = ((text_[counter] <= 'Z') && (text_[counter] >= 'A')) || ((text_[counter] <= 'z') && (text_[counter] >= 'a'));
				bool number = (text_[counter] <= '9') && (text_[counter] >= '0');
				bool underscore = text_[counter] == '_';

				if (!letter && !number && !underscore) {
					//throw an exception invalid file/folder name(forbidden character in a name)
					throw new InvalidName();
				}

				del_folder_name.push_back(text_[counter++]);
				
				if (text_[counter] == '\n') break;

				if (counter >= text_.size()) {
					end_of_string = true;
					break;
				}
			}

			Command* del_command = new Del(del_folder_name, file);
			del_command->setLogPath(log_filepath);

			this->addCommand(del_command);

			if (end_of_string) break;
			if ((text_[counter] == '\\') && (text_[counter + 1] == 'n')) counter += 2;
		}

		if ((text_[counter] == 'E') && (text_[counter + 1] == 'X') && (text_[counter + 2] == 'E')) 
		{
			counter += 3;//skips "EXE"
			while ((text_[counter] == ' ') || (text_[counter] == '\n'))
				counter++;//skips '\n' or ' '
			
			string exe_file_name;
			bool file = false, exe = false;
			while ((text_[counter] != ' ') || (text_[counter] != '\n')) {
				
				bool letter = ((text_[counter] <= 'Z') && (text_[counter] >= 'A')) || ((text_[counter] <= 'z') && (text_[counter] >= 'a'));
				bool number = (text_[counter] <= '9') && (text_[counter] >= 'A');
				bool underscore = text_[counter] == '_';

				if (!letter && !number && !underscore) {
					//throw an exception invalid file/folder name(forbidden character in a name)
					throw InvalidName();
				}
				
				exe_file_name.push_back(text_[counter++]);

				if (text_[counter] == '\n') break;

				if (text_[counter] == '.') {
					exe = (text_[counter + 1] == 'e') && (text_[counter + 2] == 'x') && (text_[counter + 3] == 'e');
					if (!exe) {
						//throw an exception: trying to call EXE on a non exe file
						throw new FileNotExecutable();
					}
					file = true;
				}

				if (counter >= text_.size()) {
					end_of_string = true;
					break;
				}
			}

			if (!file) {
				//throw an exception: EXE cannot execute directory
				throw new FolderPassedToExe();
			}

			Command* exe_command = new Exe(exe_file_name);
			exe_command->setLogPath(log_filepath);
			
			this->addCommand(exe_command);

			if (end_of_string) break;
			if ((text_[counter] == '\\') && (text_[counter + 1] == 'n')) counter += 2;
		}
		
		while((text_[counter] == ' ') || (text_[counter] == '\n')) counter++;
		
		if (counter >= text_.size()) {
			end_of_string = true;
			break;
		}
	}
}

Folder* ExeFile::execute(Folder* curr_directory)
{
	for (list<Command*>::iterator it = com_list_.begin(); it != com_list_.end(); it++) {
		curr_directory = (*it)->executeCommand(curr_directory);
	}	
	return curr_directory;
}
