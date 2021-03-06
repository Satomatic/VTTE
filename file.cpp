#include <file.h>
#include <maineditor.h>
#include <mainwindow.h>
#include <fstream>

void LoadFileNewBuffer(MainEditor *editor, std::string filename){
	std::ifstream file(filename);
	std::string cline;

	File newfile;
	newfile.filename = filename;
	newfile.savedCursorS = 0;
	newfile.savedCursorX = 0;
	newfile.savedCursorY = 0;
	newfile.savedScroll = 0;
	newfile.newfile = false;
	newfile.edited = false;

	while (getline(file, cline)){
		std::string convert = "";
		for (int i = 0; i < cline.size(); i++){
			if (cline[i] == '\t'){
				convert += "    ";
			} else if (cline[i] != '\r'){
				convert += cline[i];
			}
		}

		// @note Generating the lexer data is really slow on larger files
		newfile.FileData.push_back(convert);
		newfile.SyntaxData.push_back(editor->lexerClass.SyntaxLine(convert));
	}

	editor->files.push_back(newfile);
	file.close();
}

void SwitchFileBuffer(MainEditor* editor, int index){
	if (index < 0){
		index = editor->files.size() - 1;
	} else if (index > editor->files.size() - 1){
		index = 0;
	}

	/**
	 *  Save current editor values
	 */
	editor->files[editor->fi].savedCursorS = editor->cursors;
	editor->files[editor->fi].savedCursorX = editor->cursorx;
	editor->files[editor->fi].savedCursorY = editor->cursory;
	editor->files[editor->fi].savedScroll = editor->scroll;

	/**
	 *  Load new values from file buffer
	 */
	editor->cursors = editor->files[index].savedCursorS;
	editor->cursorx = editor->files[index].savedCursorX;
	editor->cursory = editor->files[index].savedCursorY;
	editor->scroll = editor->files[index].savedScroll;
	editor->fi = index;

	editor->updateTitle();
}

void SaveFileBuffer(MainEditor *editor, std::string filename){
	std::ofstream file(filename);

	if (file.good()){
		for (int i = 0; i < editor->files[editor->fi].FileData.size(); i++){
			std::string convert = editor->files[editor->fi].FileData[i];

			for (int b = 0; b < convert.size(); b++){
				if (convert.substr(b,4) == "    "){
					convert.replace(b, 4, "\t");
					continue;
				}
				break;
			}

			file << convert << std::endl;
		}
	}

	file.close();

	editor->files[editor->fi].edited = false;
	editor->updateTitle();
}
