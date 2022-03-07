#include <core/core.h>
#include <lexer.h>
#include <QFileDialog>
#include <file.h>

void ActionCloseFile(MainEditor *editor){
	int storedLength = editor->files.size();
	editor->files.erase(editor->files.begin() + editor->fi);
	
	/**
	 *  If file is last in the file buffer
	 */
	if (editor->fi == storedLength - 1 && editor->files.size() > 0){
		if (editor->files.size() > 0) editor->fi -= 1;
	
	/**
	 *  If there are no more files in the buffer,
	 *  just make a new file. Or close, idk yet.
	 */
	} else if (editor->files.size() == 0){
		ActionNewFile(editor);
	}
	
	editor->updateTitle();
}

void ActionNewFile(MainEditor *editor){
	File newFile;
	newFile.filename = "New file";
	newFile.FileData = {""};
	newFile.newfile = true;
	newFile.edited = true;

	/*
	 *  Make sure that all saved values are set to
	 *  0 otherwise a seg fault is most likely.
	 */
	newFile.savedCursorX = 0;
	newFile.savedCursorY = 0;
	newFile.savedCursorS = 0;
	newFile.savedScroll = 0;

	/*
	 *  Generate an initial piece of lexer data to
	 *  access later.
	 */
	newFile.SyntaxData.push_back(editor->lexerClass.SyntaxLine(newFile.FileData[0]));

	editor->files.push_back(newFile);
	SwitchFileBuffer(editor, editor->files.size() - 1);
}

void ActionOpenFile(MainEditor *editor){
	std::string filename = QFileDialog::getOpenFileName(editor->parent, editor->tr("Open source file"), "", editor->tr("All Files (*)")).toStdString();
	if (filename.size() == 0) return;

	LoadFileNewBuffer(editor, filename);
	SwitchFileBuffer(editor, editor->files.size() - 1);
}

void ActionSaveFile(MainEditor *editor){
	if (editor->files[editor->fi].newfile){
		std::string filename = QFileDialog::getSaveFileName(editor->parent, editor->tr("Save as"), "", editor->tr("All Files (*)")).toStdString();
		editor->files[editor->fi].filename = filename;
		editor->files[editor->fi].newfile = false;
	}
	SaveFileBuffer(editor, editor->files[editor->fi].filename);
}

void ActionSwitchNext(MainEditor *editor){ SwitchFileBuffer(editor, editor->fi + 1); }
void ActionSwitchPrev(MainEditor *editor){ SwitchFileBuffer(editor, editor->fi - 1); }
