#ifndef KEYBIND_H
#define KEYBIND_H

#include <global.h>

#define CTRL     67108864
#define ALT      134217728
#define CTRL_ALT 201326592

typedef void (*callback)(MainEditor*);

struct KeyBind {
    std::string key;
    int modifiers;
    callback function;
};

void InitKeybinds();

extern std::vector <KeyBind> KeyBinds;

#endif // KEYBIND_H
