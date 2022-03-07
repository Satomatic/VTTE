#ifndef CORE_H
#define CORE_H

#include <global.h>

void ActionCloseFile(MainEditor *editor);
void ActionNewFile(MainEditor *editor);
void ActionOpenFile(MainEditor *editor);
void ActionSaveFile(MainEditor *editor);
void ActionSwitchNext(MainEditor *editor);
void ActionSwitchPrev(MainEditor *editor);

#endif // CORE_H
