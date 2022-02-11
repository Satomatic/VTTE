#ifndef FILE_H
#define FILE_H

#include <string>
#include <QWidget>
#include <maineditor.h>

void LoadFileNewBuffer(MainEditor *editor, std::string filename);
void SwitchFileBuffer(MainEditor *editor, int index);
void SaveFileBuffer(MainEditor *editor, std::string filename);

#endif // FILE_H
