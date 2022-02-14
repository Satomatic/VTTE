#include <core/core.h>
#include <lexer.h>
#include <QFileDialog>
#include <file.h>

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
    LoadFileNewBuffer(editor, filename);
    SwitchFileBuffer(editor, editor->files.size() - 1);
}

void ActionSaveFile(MainEditor *editor){
    SaveFileBuffer(editor, editor->files[editor->fi].filename);
}

void ActionSwitchNext(MainEditor *editor){ SwitchFileBuffer(editor, editor->fi + 1); }
void ActionSwitchPrev(MainEditor *editor){ SwitchFileBuffer(editor, editor->fi - 1); }
