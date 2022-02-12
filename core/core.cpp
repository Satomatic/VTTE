#include <core/core.h>
#include <iostream>
#include <QFileDialog>
#include <file.h>

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
